#include "VulkanMesh.h"
#include "VulkanDescriptorPoolBuilder.h"
#include "VulkanDescriptorUtils.h"
#include "VulkanDeviceManager.h"

#include "VulkanStagingBuffer.h"
#include "VulkanTransferManager.h"
#include "VulkanPipelineManager.h"

#include "IVulkanSurfaceMaterial.h"

VulkanMesh::VulkanMesh()
{
    meshTransformBuffer.Create(sizeof(Matrix4f));
    descriptorPool = VulkanDescriptorPoolBuilder().AddUniformBuffer(1).SetMaxSets(1).Build();
    meshDescriptorSet = VulkanDescriptorUtils::AllocateSet(descriptorPool, GetVulkanPipelineManager().GetObjectDescriptorSetLayout());
}

VulkanMesh::~VulkanMesh()
{
    vertexBuffer.Cleanup();
    indexBuffer.Cleanup();
    meshTransformBuffer.Cleanup();
    material = nullptr;
}

void VulkanMesh::AddMeshPart(Vertex *vertices, uint32_t vertexCount, uint32_t *indices, uint32_t indexCount, IVulkanSurfaceMaterial *material)
{
    this->material = material;
    numIndices = indexCount;

    vertexBuffer.Cleanup();
    indexBuffer.Cleanup();

    vertexBuffer.Create(vertexCount * sizeof(Vertex));
    indexBuffer.Create(indexCount * sizeof(uint32_t));

    // Transfer vertices data to buffer
    VulkanStagingBuffer sBuffer;
    sBuffer.Create(vertexBuffer.GetBufferSize());
    sBuffer.Update(vertices);
    GetVulkanTransferManager().CopyBuffer(sBuffer.GetBuffer(), vertexBuffer.GetBuffer(), sBuffer.GetBufferSize());
    sBuffer.Cleanup();

    // Transfer indices data to buffer
    sBuffer.Create(indexBuffer.GetBufferSize());
    sBuffer.Update(indices);
    GetVulkanTransferManager().CopyBuffer(sBuffer.GetBuffer(), indexBuffer.GetBuffer(), sBuffer.GetBufferSize());
    sBuffer.Cleanup();
}

void VulkanMesh::SetTransform(const Transform &transform)
{
    auto transformMat = transform.GetTransformMatrix();
    meshTransformBuffer.Update(&transformMat);

    VkDescriptorBufferInfo bufferInfo {};
    bufferInfo.buffer = meshTransformBuffer.GetBuffer(); // Supposons que ubo expose son buffer
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(Matrix4f);

    VkWriteDescriptorSet descriptorWrite {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = meshDescriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), 1, &descriptorWrite, 0, nullptr);
}

void VulkanMesh::Draw(VkCommandBuffer commandBuffer)
{
    // TODO: Add a default engine material
    if (!material)
        return;

    // TODO: Separate material pass selection with differents functions
    material->Bind(commandBuffer);
    
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetVulkanPipelineManager().GetPipelineLayout(), 1, 1, &meshDescriptorSet, 0, nullptr);

    VkDeviceSize offset = 0;

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.GetBuffer(), &offset);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, numIndices, 1, 0, 0, 0);
}

void VulkanMesh::DrawForShadowMap(VkCommandBuffer commandBuffer)
{

    // TODO: Add a default engine material
    if (!material)
        return;

    // TODO: Separate material pass selection with differents functions
    material->BindForShadowMap(commandBuffer);
    
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetVulkanPipelineManager().GetPipelineLayout(), 1, 1, &meshDescriptorSet, 0, nullptr);

    VkDeviceSize offset = 0;

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.GetBuffer(), &offset);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, numIndices, 1, 0, 0, 0);
}
