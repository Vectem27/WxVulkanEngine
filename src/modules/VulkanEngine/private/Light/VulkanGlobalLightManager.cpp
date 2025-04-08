#include "VulkanGlobalLightManager.h"
#include "VulkanRenderEngine.h"
#include "VulkanSpotlightLight.h"
#include "UniformBufferObject.h"

void VulkanGlobalLightManager::InitVulkanLightManager(VulkanRenderEngine *vulkanRenderEngine)
{
    if (!vulkanRenderEngine)
        throw std::invalid_argument("Render engine reference is null");

    this->vulkanRenderEngine = vulkanRenderEngine;

    CreateDescriptorPool();
    CreateDescriptorSets(); 

    isInitialized = true;
}

void VulkanGlobalLightManager::ClearLights()
{
    projectorLights.Clear();
}

void VulkanGlobalLightManager::UpdateDescriptorSets()
{
    if (!IsInitialized())
    {
        throw std::runtime_error("Vulkan light manager is not initialized");
        return;
    }

    Array<VertexLightData> vertexLights;
    Array<FragmentLightData> fragLights;
    for(const auto& light : projectorLights)
    {
        if(!light)
            continue;
        VertexLightData vdata;
        FragmentLightData fdata;
        vdata.viewProj = light->GetSpotlightLightData().viewProj;
        fdata.pos = light->GetSpotlightLightData().position;
        fdata.direction = light->GetSpotlightLightData().direction;
        fdata.length = light->GetSpotlightLightData().length;
        fdata.angle = light->GetSpotlightLightData().angle;
        vertexLights.Add(vdata);
        fragLights.Add(fdata);
    }

    std::vector<VkWriteDescriptorSet> descriptorWrites;

    /* VERTEX */
    UniformBuffer lightVertexBuffer;
    size_t lightVertexBufferSize = vertexLights.GetSize() * sizeof(VertexLightData);
    lightVertexBuffer.Create(GetRenderEngine()->GetDevice(), GetRenderEngine()->GetPhysicalDevice(), lightVertexBufferSize);

    lightVertexBuffer.Update(vertexLights.GetData());

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = lightVertexBuffer.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = lightVertexBufferSize;

    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = projectorLightsDescriptorSet;
    writeDescriptorSet.dstBinding = 1;  // Binding des lumières
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writeDescriptorSet.pBufferInfo = &bufferInfo;
    writeDescriptorSet.descriptorCount = 1;

    descriptorWrites.push_back(writeDescriptorSet);

    /* FRAGMENT */
    UniformBuffer lightFragBuffer;
    size_t lightFragBufferSize = fragLights.GetSize() * sizeof(FragmentLightData);
    lightFragBuffer.Create(GetRenderEngine()->GetDevice(), GetRenderEngine()->GetPhysicalDevice(), lightFragBufferSize);

    lightFragBuffer.Update(fragLights.GetData());

    VkDescriptorBufferInfo bufferInfo2 = {};
    bufferInfo2.buffer = lightFragBuffer.GetBuffer();
    bufferInfo2.offset = 0;
    bufferInfo2.range = lightFragBufferSize;

    VkWriteDescriptorSet writeDescriptorSet2 = {};
    writeDescriptorSet2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet2.dstSet = projectorLightsDescriptorSet;
    writeDescriptorSet2.dstBinding = 2;  // Binding des lumières
    writeDescriptorSet2.dstArrayElement = 0;
    writeDescriptorSet2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writeDescriptorSet2.pBufferInfo = &bufferInfo2;
    writeDescriptorSet2.descriptorCount = 1;

    descriptorWrites.push_back(writeDescriptorSet2);
    /* SHADOW MAPS */

    if (projectorLights.GetSize() <= 0)
        throw std::runtime_error("Projector lights empty");

    if (projectorLights[0]->GetSpotlightLightData().shadowMapsInfo.GetSize() <= 0)
        throw std::runtime_error("Projector lights empty shadow maps info");

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    imageInfo.imageView = projectorLights[0]->GetSpotlightLightData().shadowMapsInfo[0].imageView;
    imageInfo.sampler = projectorLights[0]->GetSpotlightLightData().shadowMapsInfo[0].sampler;

    
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

    lightVertexBuffer.Cleanup();
    lightFragBuffer.Cleanup();
}

void VulkanGlobalLightManager::AddProjectorLight(const VulkanSpotlightLight *light)
{
    if (light)
        projectorLights.Add(light);
}

void VulkanGlobalLightManager::CreateDescriptorPool()
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

void VulkanGlobalLightManager::CreateDescriptorSets()
{
    VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &vulkanRenderEngine->GetPipelineManager()->GetShadowMapDescriptorSetLayout();

        if (!vulkanRenderEngine->GetDescriptorPoolManager()->AllocateDescriptorSets(&vulkanRenderEngine->GetPipelineManager()->GetShadowMapDescriptorSetLayout(), 1, &projectorLightsDescriptorSet))
            throw std::runtime_error("Failed to allocate descriptor set !");
    
}
