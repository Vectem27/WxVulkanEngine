#include "LightManagers/VulkanSpotlightLightManager.h"

#include <vector>
#include <array>

#include "VulkanSpotlightLight.h"
#include "VulkanSpotlightLightPipeline.h"

#include "VulkanDeviceManager.h"
#include "VulkanPipelineManager.h"

void VulkanSpotlightLightManager::InitLightManager()
{
    CreateDescriptorPool();
    CreateDescriptorSets();

    lightDataBuffer.Create( sizeof(LightData) * GetMaxNumOfLights());
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
    int num = 0; //fragLights.GetSize();
    for(const auto& light : lights)
    {
        if(!light) continue;

        LightData fdata;
        fdata.viewProj = light->GetLightViewProj();
        fdata.pos = light->GetLightPosition();
        fdata.direction = light->GetLightDirection();
        fdata.length = light->GetLightLength();
        fdata.angle = light->GetLightAngle();
        fdata.color = light->GetLightColor();
        fdata.intensity = light->GetLightIntensity();
        fdata.softAngle = light->GetLightSoftAngle();
        fdata.shadowMapIndex = num;

        lightDataBuffer.Update(&fdata, sizeof(LightData) * num, sizeof(LightData));
        ++num;
    }

    std::vector<VkWriteDescriptorSet> descriptorWrites;
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = lightDataBuffer.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = lightDataBuffer.GetBufferSize();

    descriptorWrites.push_back((VkWriteDescriptorSet){
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = descriptorSet,
        .dstBinding = 0,  
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        .pBufferInfo = &bufferInfo,
    });

    /* SHADOW MAPS */
    std::vector<VkDescriptorImageInfo> imageInfo;
    for (size_t i = 0; i < lights.GetSize(); i++)
    {
        imageInfo.push_back({
            .sampler = VulkanPipelineManager::GetInstance().GetShadowMapSampler(),
            .imageView = lights[i]->GetSpotlightLightData().shadowMapView,
            .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
        });
    }

    descriptorWrites.push_back((VkWriteDescriptorSet){
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = descriptorSet,
        .dstBinding = 1,
        .dstArrayElement = 0,
        .descriptorCount = static_cast<uint32_t>(imageInfo.size()),
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo = imageInfo.data(),
    });

    vkUpdateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}

void VulkanSpotlightLightManager::Bind(VkCommandBuffer commandBuffer) const
{
    for (size_t i = 0; i < GetLightCount(); i++)
    {
        uint32_t offset = i * sizeof(LightData);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanPipelineManager::GetInstance().GetLightingPipelineLayout(), 1, 1, &descriptorSet, 1, &offset);
        vkCmdDraw(commandBuffer, 4, 1, 0, 0);
    }
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

    if (vkCreateDescriptorPool(GetVulkanDeviceManager().GetDeviceChecked(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create spotlight manager descriptor pool");
}

void VulkanSpotlightLightManager::CreateDescriptorSets()
{
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &VulkanPipelineManager::GetInstance().GetLightDescriptorSetLayout();
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = descriptorPool;

    VkResult allocResult = vkAllocateDescriptorSets(
        GetVulkanDeviceManager().GetDeviceChecked(), &allocInfo, &descriptorSet
    );
    
    if (allocResult != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate spotlight manager descriptor set");
}
