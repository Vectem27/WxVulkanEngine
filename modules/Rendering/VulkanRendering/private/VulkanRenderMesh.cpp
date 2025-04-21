#include "VulkanRenderMesh.h"

// RHI
#include "IRenderMaterial.h"

// VulkanAPI
#include "VulkanDescriptorPoolBuilder.h"
#include "VulkanDescriptorUtils.h"
#include "VulkanPipelineManager.h"

#include "VulkanStagingBuffer.h"
#include "VulkanTransferManager.h" // Transfer data from cpu buffer (staging) to gpu buffer (vertex buffer, index buffer, storage buffer, etc...)

// VulkanRendering
#include "VulkanRenderInfo.h"
#include "VulkanSurfaceRenderMaterial.h"

#include <thread>


Vulkan::RenderMesh::RenderMesh()
{
    instanceDescriptorPool = VulkanDescriptorPoolBuilder().SetMaxSets(1).AddUniformBufferDynamic(1).Build();
    instanceDescriptorSet = VulkanDescriptorUtils::AllocateSet(
        instanceDescriptorPool, 
        GetVulkanPipelineManager().GetObjectDescriptorSetLayout() // TODO: Change to a storage buffer and change the descriptor set layout
    );
}

Vulkan::RenderMesh::~RenderMesh()
{
    VulkanDescriptorUtils::DestroyDescriptorPool(instanceDescriptorPool);
}

void Vulkan::RenderMesh::Render(const void *renderInfo) const
{
    auto vulkanRenderInfo = static_cast<const RenderInfo*>(renderInfo);

    if (!vulkanRenderInfo)
    {
        Log(Warning, "Vulkan", "Failed to render mesh : vulkan render info is null");
        return;
    }

    if (!vulkanRenderInfo->commandBuffer)
    {
        Log(Warning, "Vulkan", "Failed to render mesh : command buffer is null");
        return;
    }

    for(int i = 0; i < indexBuffers.GetSize(); ++i)
    {
        RenderMeshPart(vulkanRenderInfo->commandBuffer, i);
    }   
}

uint32_t Vulkan::RenderMesh::AddInstance(const Transform &transform)
{
    // Increment at each call to AddInstance and avoid collision with existing ids
    static uint32_t instanceId{0};

    std::lock_guard<std::mutex> lock(instanceMutex);
    MeshInstanceData instanceData;
    instanceData.transformMatrix = transform.GetTransformMatrix();

    instances.insert({instanceId, instanceData});

    return instanceId++; // Return the id of the instance
}

void Vulkan::RenderMesh::RemoveInstance(uint32_t instanceId)
{
    if (auto it = instances.find(instanceId); it != instances.end())
        instances.erase(it);
    else
        Log(Warning, "Vulkan", "Failed to remove instance : instance not found");
}

void Vulkan::RenderMesh::SetInstanceTransform(uint32_t instanceId, const Transform &transform)
{
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (auto it = instances.find(instanceId); it != instances.end())
        it->second.transformMatrix = transform.GetTransformMatrix();
    else
        Log(Warning, "Vulkan", "Failed to set instance transform : instance not found");
}

void Vulkan::RenderMesh::UpdateRenderedInstances(bool updateAsync)
{
    if(updateAsync)
    {
        std::thread([this]() {
            this->UpdateInstanceDataBuffer();
        }).detach();
    }
    else
        UpdateInstanceDataBuffer();
}

void Vulkan::RenderMesh::ClearInstances()
{
    std::lock_guard<std::mutex> lock(instanceMutex);
    instances.clear();
}

void Vulkan::RenderMesh::SetMaterial(uint32_t index, const RHI::IRenderMaterial *material)
{
    if (const SurfaceRenderMaterial* surfaceMaterial = dynamic_cast<const SurfaceRenderMaterial*>(material); surfaceMaterial)
    {
        constexpr bool sizeToFit{true};
        materials.Insert(surfaceMaterial, index, sizeToFit);
    }
    else
    {
        Log(Warning, "Vulkan", "Failed to set mesh material : material is not a VulkanSurfaceMaterial");
    }
}

void Vulkan::RenderMesh::SetVertices(Vertex *vertices, uint32_t vertexCount)
{
    VulkanVertexBuffer vertexBuffer;
    vertexBuffer.Create(vertexCount * sizeof(Vertex));

    VulkanStagingBuffer stagingBuffer;
    stagingBuffer.Create(vertexBuffer.GetBufferSize());
    stagingBuffer.Update(vertices);

    GetVulkanTransferManager().CopyBuffer(
        stagingBuffer.GetBuffer(), 
        vertexBuffer.GetBuffer(), 
        vertexBuffer.GetBufferSize()
    );

    stagingBuffer.Cleanup();

    this->vertexBuffer.Cleanup();
    this->vertexBuffer = std::move(vertexBuffer);
}

void Vulkan::RenderMesh::AddMeshPart(uint32_t *indices, uint32_t indexCount)
{
    VulkanIndexBuffer indexBuffer;
    indexBuffer.Create(indexCount * sizeof(uint32_t));

    VulkanStagingBuffer stagingBuffer;
    stagingBuffer.Create(indexBuffer.GetBufferSize());
    stagingBuffer.Update(indices);

    GetVulkanTransferManager().CopyBuffer(
        stagingBuffer.GetBuffer(), 
        indexBuffer.GetBuffer(), 
        indexBuffer.GetBufferSize()
    );

    stagingBuffer.Cleanup();

    indexBuffers.Add(std::move(indexBuffer));
}

void Vulkan::RenderMesh::RenderMeshPart(VkCommandBuffer commandBuffer, uint32_t meshPart) const
{
    if (meshPart >= indexBuffers.GetSize())
    {
        Log(Warning, "Vulkan", "Failed to render mesh part : mesh part index out of range");
        return;
    }

    if (meshPart >= materials.GetSize()) return;

    const auto& indexBuffer = indexBuffers[meshPart];
    const auto& material = materials[meshPart];

    if (!material) return; // TODO: Render a default material instead

    material->BindRenderMaterial(commandBuffer);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetVulkanPipelineManager().GetPipelineLayout(), 1, 1, &instanceDescriptorSet, 0, nullptr);

    VkDeviceSize offset = 0;
    auto vBuffer = vertexBuffer.GetBuffer();
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vBuffer, &offset);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, indexBuffer.GetBufferSize() / sizeof(uint32_t), instances.size(), 0, 0, 0);
}

void Vulkan::RenderMesh::UpdateInstanceDataBuffer()
{
    std::lock_guard<std::mutex> lock(instanceMutex);

    Array<MeshInstanceData> instancesDataArray;
    for (auto& instance : instances)
        instancesDataArray.Add(instance.second);

    instanceDataBuffer.Cleanup();
    instanceDataBuffer.Create(instancesDataArray.GetSize() * sizeof(MeshInstanceData));

    VulkanStagingBuffer stagingBuffer;
    stagingBuffer.Create(instanceDataBuffer.GetBufferSize());
    stagingBuffer.Update(instancesDataArray.GetData());
    GetVulkanTransferManager().CopyBuffer(
        stagingBuffer.GetBuffer(), 
        instanceDataBuffer.GetBuffer(), 
        instanceDataBuffer.GetBufferSize()
    );
    stagingBuffer.Cleanup();


    VulkanDescriptorUtils::UpdateSet(
        instanceDescriptorSet, 
        instanceDataBuffer,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        0 // binding
    );
}
