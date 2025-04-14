#include "Mesh.h"

#include "VulkanRenderEngine.h"
#include "VulkanPipelineManager.h"

struct ObjectData
{
    alignas(16) Matrix4f model;
};

Mesh::~Mesh()
{
    if (IsVulkanInitialized())
        GetVulkanMeshBuffer().Cleanup();
}

void Mesh::SetMeshTransform(Transform newTransform)
{
    meshTransform = newTransform;
    
    if (IsVulkanInitialized())
        UpdateVulkanMeshBuffer();
}

void Mesh::InitVulkanMesh(VulkanRenderEngine *vulkanRenderEngine)
{
    if (!vulkanRenderEngine)
        throw std::invalid_argument("Render engine reference is null");

    this->vulkanRenderEngine = vulkanRenderEngine;

    if (!vulkanRenderEngine->GetDescriptorPoolManager()->AllocateDescriptorSets(&GetVulkanPipelineManager().GetObjectDescriptorSetLayout(), 1, &GetVulkanMeshDescriptorSet()))
        throw std::runtime_error("Failed to allocate descriptor set !");


    GetVulkanMeshBuffer().Create(sizeof(ObjectData));

    UpdateVulkanMeshBuffer();

    isVulkanInitialized = true;
}

void Mesh::UpdateVulkanMeshBuffer()
{
    auto transformMat = GetMeshTransform().GetTransformMatrix();
    
    GetVulkanMeshBuffer().Update(&transformMat);

    VkDescriptorBufferInfo bufferInfo {};
    bufferInfo.buffer = GetVulkanMeshBuffer().GetBuffer(); // Supposons que ubo expose son buffer
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(ObjectData);

    VkWriteDescriptorSet descriptorWrite {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = GetVulkanMeshDescriptorSet();
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), 1, &descriptorWrite, 0, nullptr);
}
