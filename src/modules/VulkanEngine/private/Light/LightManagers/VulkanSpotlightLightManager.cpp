#include "LightManagers/VulkanSpotlightLightManager.h"

#include <vector>
#include <array>

#include "Matrix.hpp"

#include "VulkanSpotlightLight.h"
#include "VulkanSpotlightLightPipeline.h"

#include "VulkanDeviceManager.h"
#include "VulkanPipelineManager.h"

#include "VulkanDescriptorPoolBuilder.h"
#include "VulkanDescriptorUtils.h"

/**
 * @struct The light data struct aligned to be transfered in shaders
 */
struct alignas(256) LightData
{
    alignas(16) Matrix4f viewProj;
    alignas(16) Vector3f pos;
    alignas(16) Vector3f direction;
    alignas(4)  float length;
    alignas(4)  float angle;
    alignas(16) Vector3f color;
    alignas(4)  float intensity;
    alignas(4)  float softAngle; // Angle de transition douce
    alignas(4)  unsigned int shadowMapIndex;
};

VulkanSpotlightLightManager::VulkanSpotlightLightManager()
{
    descriptorPool = VulkanDescriptorPoolBuilder().SetMaxSets(16).AddUniformBufferDynamic(1).AddCombinedImageSampler(GetVulkanPipelineManager().GetMaxLightsPerDescriptor()).Build();
    descriptorSet = VulkanDescriptorUtils::AllocateSet(descriptorPool, GetVulkanPipelineManager().GetLightDescriptorSetLayout());

    lightDataBuffer.Create(sizeof(LightData) * GetVulkanPipelineManager().GetMaxLightsPerDescriptor());

    spotlightLightPipeline = new VulkanSpotlightLightPipeline();
    spotlightLightPipeline->InitPipeline(GetVulkanDeviceManager().GetDeviceChecked(), &VulkanPipelineManager::GetInstance(),
        "shaders/fullScreen.vert", "shaders/lighting.frag");

}

VulkanSpotlightLightManager::~VulkanSpotlightLightManager()
{
    delete spotlightLightPipeline;
    lightDataBuffer.Cleanup();

    VulkanDescriptorUtils::DestroyDescriptorPool(descriptorPool);
}

void VulkanSpotlightLightManager::AddLight(const IVulkanLight *light)
{
    if (light->GetLightType() != VulkanSpotlightLight::lightType)
        return;

    auto spotlightLight = dynamic_cast<const VulkanSpotlightLight*>(light);
    
    if (spotlightLight)
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
    spotlightLightPipeline->Bind(commandBuffer);

    for (size_t i = 0; i < GetLightCount(); i++)
    {
        uint32_t offset = i * sizeof(LightData);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanPipelineManager::GetInstance().GetLightingPipelineLayout(), 1, 1, &descriptorSet, 1, &offset);
        vkCmdDraw(commandBuffer, 4, 1, 0, 0);
    }
}

