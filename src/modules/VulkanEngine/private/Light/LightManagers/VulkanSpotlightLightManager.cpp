#include "LightManagers/VulkanSpotlightLightManager.h"

#include <vector>

#include "VulkanSpotlightLight.h"
#include "VulkanRenderEngine.h"
#include "VulkanSpotlightLightPipeline.h"

void VulkanSpotlightLightManager::InitLightManager(VulkanRenderEngine *vulkanRenderEngine)
{
    if (!vulkanRenderEngine)
            throw std::invalid_argument("Render engine reference is null");

    this->vulkanRenderEngine = vulkanRenderEngine;
    CreateDescriptorPool();
    CreateDescriptorSets();

    lightDataBuffer.Create(
        vulkanRenderEngine->GetDevice(),
        vulkanRenderEngine->GetPhysicalDevice(),
        sizeof(LightData) * GetMaxNumOfLights() + 16
    );

    
    isInitialized = true;
}

void VulkanSpotlightLightManager::AddLight(const IVulkanLight *light)
{
    if (light->GetLightType() != VulkanSpotlightLight::lightType)
        return;

    auto spotlightLight = dynamic_cast<const VulkanSpotlightLight*>(light);
    
    if (spotlightLight && GetLightCount() < GetMaxNumOfLights())
        lights.Add(spotlightLight);
}

void VulkanSpotlightLightManager::Update()
{
    if (!IsInitialized())
        throw std::runtime_error("Vulkan spotlight light manager is not initialized");

    Array<LightData> fragLights;
    for(const auto& light : lights)
    {
        if(!light) continue;

        LightData fdata;
        fdata.viewProj = light->GetSpotlightLightData().viewProj;
        fdata.pos = light->GetSpotlightLightData().position;
        fdata.direction = light->GetSpotlightLightData().direction;
        fdata.length = light->GetSpotlightLightData().length;
        fdata.angle = light->GetSpotlightLightData().angle;
        fdata.color = {1.0f, 1.0f, 1.0f};
        fdata.intensity = 2.0f;
        fdata.penumbraAngle = ToRadians(5.0f);
        fragLights.Add(fdata);
    }

    std::vector<VkWriteDescriptorSet> descriptorWrites;

    
    /* FRAGMENT */
    int num = 1; //fragLights.GetSize();
    lightDataBuffer.Update(&num, 0, 4);
    lightDataBuffer.Update(fragLights.GetData(), 16, fragLights.GetSize() * sizeof(LightData));

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = lightDataBuffer.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = lightDataBuffer.GetBufferSize();

    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = descriptorSet;
    writeDescriptorSet.dstBinding = 0;  // Binding des lumières
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writeDescriptorSet.pBufferInfo = &bufferInfo;
    writeDescriptorSet.descriptorCount = 1;

    descriptorWrites.push_back(writeDescriptorSet);
    /* SHADOW MAPS */

    if (lights.GetSize() <= 0)
        throw std::runtime_error("Projector lights empty");

    if (lights[0]->GetSpotlightLightData().shadowMapsInfo.GetSize() <= 0)
        throw std::runtime_error("Projector lights empty shadow maps info");

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    imageInfo.imageView = lights[0]->GetSpotlightLightData().shadowMapsInfo[0].imageView;
    imageInfo.sampler = lights[0]->GetSpotlightLightData().shadowMapsInfo[0].sampler;
    
    VkWriteDescriptorSet desc{};
    desc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    desc.dstSet = descriptorSet;
    desc.dstBinding = 1;
    desc.dstArrayElement = 0;
    desc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    desc.descriptorCount = 1;
    desc.pImageInfo = &imageInfo;

    descriptorWrites.push_back(desc);


    vkUpdateDescriptorSets(GetRenderEngine()->GetDevice(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

}

void VulkanSpotlightLightManager::Bind(VkCommandBuffer commandBuffer) const
{
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetRenderEngine()->GetPipelineManager()->GetLightingPipelineLayout(), 1, 1, &descriptorSet, 0, nullptr);
}

void VulkanSpotlightLightManager::CreateDescriptorPool() 
{
    std::vector<VkDescriptorPoolSize> poolSizes = {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, GetMaxNumOfLights() },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, GetMaxNumOfLights() },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, GetMaxNumOfLights() }
    };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 2;

    if (vkCreateDescriptorPool(GetRenderEngine()->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create spotlight manager descriptor pool");
}

void VulkanSpotlightLightManager::CreateDescriptorSets()
{
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &GetRenderEngine()->GetPipelineManager()->GetLightDescriptorSetLayout();
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = descriptorPool;

    VkResult allocResult = vkAllocateDescriptorSets(
        GetRenderEngine()->GetDevice(), &allocInfo, &descriptorSet
    );
    
    if (allocResult != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate spotlight manager descriptor set");
}
