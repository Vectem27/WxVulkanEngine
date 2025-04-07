#ifndef VULKANLIGHT_H
#define VULKANLIGHT_H

#include <vulkan/vulkan.h>
#include "VulkanCamera.h"
#include <stdexcept>
#include "VulkanRenderEngine.h"
#include "UniformBufferObject.h" 

class VulkanLight
{
public:
    ~VulkanLight()
    {
        if (lightDescriptorPool != VK_NULL_HANDLE) 
        {
            vkDestroyDescriptorPool(renderEngine->GetDevice(), lightDescriptorPool, nullptr);
            lightDescriptorPool = VK_NULL_HANDLE;
        }
    }
public:
    void InitVulkanLight(VulkanRenderEngine* renderEngine)
    {
        if (!renderEngine)
        { 
            throw std::invalid_argument("Vulkan render engine is invalid !");
            return;
        }

        this->renderEngine = renderEngine;

        CreateDescriptorPool();
        CreateDescriptorSet();

        viewBuffer.Create(this->renderEngine->GetDevice(), this->renderEngine->GetPhysicalDevice(), sizeof(ViewProj));

        isInitialized = true;
    }

    void SetShadowMap(VkImageView shadowMapView, VkSampler shadowMapSampler, ViewProj viewData)
    {
        if (!IsInitialized())
        {
            throw std::runtime_error("Vulkan light is not initialized");
            return;
        }

        viewBuffer.Update(&viewData);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        imageInfo.imageView = shadowMapView;
        imageInfo.sampler = shadowMapSampler;

        std::vector<VkWriteDescriptorSet> descriptorWrites;
        
        VkWriteDescriptorSet desc1{};
        desc1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc1.dstSet = GetLightDescriptorSet();
        desc1.dstBinding = 0;
        desc1.dstArrayElement = 0;
        desc1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        desc1.descriptorCount = 1;
        desc1.pImageInfo = &imageInfo;

        descriptorWrites.push_back(desc1);

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = viewBuffer.GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(ViewProj);
    
        VkWriteDescriptorSet desc2{};
        desc2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc2.dstSet = GetLightDescriptorSet();
        desc2.dstBinding = 1;
        desc2.dstArrayElement = 0;
        desc2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        desc2.descriptorCount = 1;
        desc2.pBufferInfo = &bufferInfo;

        descriptorWrites.push_back(desc2);

        vkUpdateDescriptorSets(renderEngine->GetDevice(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
    }

    bool IsInitialized() const { return isInitialized; }

    const VkDescriptorSet& GetLightDescriptorSet() const { return lightDescriptorSet; }
private:
    void CreateDescriptorPool()
    {
        std::vector<VkDescriptorPoolSize> poolSizes = {
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 }
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = 10;

        if (vkCreateDescriptorPool(renderEngine->GetDevice(), &poolInfo, nullptr, &lightDescriptorPool) != VK_SUCCESS)
            throw std::runtime_error("Échec de la création du descriptor pool !");
    }

    void CreateDescriptorSet()
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &renderEngine->GetPipelineManager()->GetShadowMapDescriptorSetLayout();

        if (!renderEngine->GetDescriptorPoolManager()->AllocateDescriptorSets(&renderEngine->GetPipelineManager()->GetShadowMapDescriptorSetLayout(), 1, &lightDescriptorSet))
            throw std::runtime_error("Failed to allocate descriptor set !");
    }
private: 
    bool isInitialized;
    VulkanRenderEngine* renderEngine{nullptr};
    VkDescriptorSet lightDescriptorSet{VK_NULL_HANDLE};
    VkDescriptorPool lightDescriptorPool{VK_NULL_HANDLE};

    UniformBuffer viewBuffer;
};

#endif // VULKANLIGHT_H