#include "VulkanLightManager.h"
#include "VulkanRenderEngine.h"
#include "VulkanProjectorLight.h"
#include "UniformBufferObject.h"

void VulkanLightManager::InitVulkanLightManager(VulkanRenderEngine *vulkanRenderEngine)
{
    if (!vulkanRenderEngine)
        throw std::invalid_argument("Render engine reference is null");

    this->vulkanRenderEngine = vulkanRenderEngine;

    CreateDescriptorPool();
    CreateDescriptorSets(); 

    isInitialized = true;
}

void VulkanLightManager::ClearLights()
{
    projectorLights.Clear();
}

void VulkanLightManager::UpdateDescriptorSets()
{
    if (!IsInitialized())
    {
        throw std::runtime_error("Vulkan light manager is not initialized");
        return;
    }

    Array<LightData> lights;
    for(const auto& light : projectorLights)
    {
        if(!light)
            continue;
        LightData data;
        data.viewProj = light->GetProjectorLightData().viewProj;
        lights.Add(data);
    }

    std::vector<VkWriteDescriptorSet> descriptorWrites;

    size_t lightBufferSize = lights.GetSize() * sizeof(LightData);
    UniformBuffer lightBuffer;
    lightBuffer.Create(GetRenderEngine()->GetDevice(), GetRenderEngine()->GetPhysicalDevice(), lightBufferSize);

    lightBuffer.Update(lights.GetData());

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = lightBuffer.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = lightBufferSize;

    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = projectorLightsDescriptorSet;
    writeDescriptorSet.dstBinding = 1;  // Binding des lumières
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writeDescriptorSet.pBufferInfo = &bufferInfo;
    writeDescriptorSet.descriptorCount = 1;

    descriptorWrites.push_back(writeDescriptorSet);

    vkUpdateDescriptorSets(GetRenderEngine()->GetDevice(), 1, &writeDescriptorSet, 0, nullptr);

    lightBuffer.Cleanup();

    

    if (projectorLights.GetSize() <= 0)
        throw std::runtime_error("Projector lights empty");

    if (projectorLights[0]->GetProjectorLightData().shadowMapsInfo.GetSize() <= 0)
        throw std::runtime_error("Projector lights empty shadow maps info");

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    imageInfo.imageView = projectorLights[0]->GetProjectorLightData().shadowMapsInfo[0].imageView;
    imageInfo.sampler = projectorLights[0]->GetProjectorLightData().shadowMapsInfo[0].sampler;

    
    VkWriteDescriptorSet desc1{};
    desc1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    desc1.dstSet = GetProjectorLightsDescriptorSet();
    desc1.dstBinding = 0;
    desc1.dstArrayElement = 0;
    desc1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    desc1.descriptorCount = 1;
    desc1.pImageInfo = &imageInfo;

    descriptorWrites.push_back(desc1);

    vkUpdateDescriptorSets(GetRenderEngine()->GetDevice(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}

void VulkanLightManager::AddProjectorLight(const VulkanProjectorLight *light)
{
    projectorLights.Add(light);
}

void VulkanLightManager::CreateDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes = {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 20 }
    };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 10;

    if (vkCreateDescriptorPool(vulkanRenderEngine->GetDevice(), &poolInfo, nullptr, &projectorLightsDescriptorPool) != VK_SUCCESS)
        throw std::runtime_error("Échec de la création du descriptor pool !");
}

void VulkanLightManager::CreateDescriptorSets()
{
    VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &vulkanRenderEngine->GetPipelineManager()->GetShadowMapDescriptorSetLayout();

        if (!vulkanRenderEngine->GetDescriptorPoolManager()->AllocateDescriptorSets(&vulkanRenderEngine->GetPipelineManager()->GetShadowMapDescriptorSetLayout(), 1, &projectorLightsDescriptorSet))
            throw std::runtime_error("Failed to allocate descriptor set !");
    
}
