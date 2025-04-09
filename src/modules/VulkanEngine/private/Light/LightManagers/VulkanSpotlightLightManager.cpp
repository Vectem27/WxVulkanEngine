#include "LightManagers/VulkanSpotlightLightManager.h"

#include <vector>

#include "VulkanSpotlightLight.h"
#include "VulkanRenderEngine.h"

void VulkanSpotlightLightManager::InitLightManager(VulkanRenderEngine *vulkanRenderEngine)
{
    if (!vulkanRenderEngine)
            throw std::invalid_argument("Render engine reference is null");

    this->vulkanRenderEngine = vulkanRenderEngine;
    CreateDescriptorPool();
    CreateDescriptorSets();

    lightsBuffer.vert.Create(
        vulkanRenderEngine->GetDevice(),
        vulkanRenderEngine->GetPhysicalDevice(),
        sizeof(VertexShaderLightData) * GetMaxNumOfLights() + sizeof(unsigned int)
    );

    lightsBuffer.frag.Create(
        vulkanRenderEngine->GetDevice(),
        vulkanRenderEngine->GetPhysicalDevice(),
        sizeof(FragmentShaderLightData) * GetMaxNumOfLights() + sizeof(unsigned int)
    );

    isInitialized = true;
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
    if (!IsInitialized())
        throw std::runtime_error("Vulkan spotlight light manager is not initialized");

    Array<VertexShaderLightData> vertexLights;
    Array<FragmentShaderLightData> fragLights;
    for(const auto& light : lights)
    {
        if(!light) continue;


        VertexShaderLightData vdata;
        vdata.viewProj = light->GetSpotlightLightData().viewProj;
        vertexLights.Add(vdata);

        FragmentShaderLightData fdata;
        fdata.pos = light->GetSpotlightLightData().position;
        fdata.direction = light->GetSpotlightLightData().direction;
        fdata.length = light->GetSpotlightLightData().length;
        fdata.angle = light->GetSpotlightLightData().angle;
        fragLights.Add(fdata);
    }

    std::vector<VkWriteDescriptorSet> descriptorWrites;

    /* VERTEX */
    lightsBuffer.vert.Update(vertexLights.GetData());

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = lightsBuffer.vert.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = lightsBuffer.vert.GetBufferSize();

    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = descriptorSet;
    writeDescriptorSet.dstBinding = 1;  // Binding des lumières
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writeDescriptorSet.pBufferInfo = &bufferInfo;
    writeDescriptorSet.descriptorCount = 1;

    descriptorWrites.push_back(writeDescriptorSet);

    /* FRAGMENT */
    lightsBuffer.frag.Update(fragLights.GetData());

    VkDescriptorBufferInfo bufferInfo2 = {};
    bufferInfo2.buffer = lightsBuffer.frag.GetBuffer();
    bufferInfo2.offset = 0;
    bufferInfo2.range = lightsBuffer.frag.GetBufferSize();

    VkWriteDescriptorSet writeDescriptorSet2 = {};
    writeDescriptorSet2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet2.dstSet = descriptorSet;
    writeDescriptorSet2.dstBinding = 2;  // Binding des lumières
    writeDescriptorSet2.dstArrayElement = 0;
    writeDescriptorSet2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writeDescriptorSet2.pBufferInfo = &bufferInfo2;
    writeDescriptorSet2.descriptorCount = 1;

    descriptorWrites.push_back(writeDescriptorSet2);
    /* SHADOW MAPS */

    if (lights.GetSize() <= 0)
        throw std::runtime_error("Projector lights empty");

    if (lights[0]->GetSpotlightLightData().shadowMapsInfo.GetSize() <= 0)
        throw std::runtime_error("Projector lights empty shadow maps info");

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    imageInfo.imageView = lights[0]->GetSpotlightLightData().shadowMapsInfo[0].imageView;
    imageInfo.sampler = lights[0]->GetSpotlightLightData().shadowMapsInfo[0].sampler;

    
    VkWriteDescriptorSet desc1{};
    desc1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    desc1.dstSet = descriptorSet;
    desc1.dstBinding = 0;
    desc1.dstArrayElement = 0;
    desc1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    desc1.descriptorCount = 1;
    desc1.pImageInfo = &imageInfo;

    descriptorWrites.push_back(desc1);


    vkUpdateDescriptorSets(GetRenderEngine()->GetDevice(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

}

void VulkanSpotlightLightManager::Bind(VkCommandBuffer commandBuffer) const
{
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetRenderEngine()->GetPipelineManager()->GetPipelineLayout(), 2, 1, &descriptorSet, 0, nullptr);
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
    poolInfo.maxSets = 1;

    if (vkCreateDescriptorPool(GetRenderEngine()->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create spotlight manager descriptor pool");
}

void VulkanSpotlightLightManager::CreateDescriptorSets()
{
    bool allocResult = GetRenderEngine()->GetDescriptorPoolManager()->AllocateDescriptorSets(
        &GetRenderEngine()->GetPipelineManager()->GetShadowMapDescriptorSetLayout(), 
        1, 
        &descriptorSet
    );

    if (!allocResult)
        throw std::runtime_error("Failed to allocate spotlight manager descriptor set");
}
