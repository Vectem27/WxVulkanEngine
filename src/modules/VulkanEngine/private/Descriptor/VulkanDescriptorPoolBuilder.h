#ifndef VULKANDESCRIPTORPOOLBUILDER_H
#define VULKANDESCRIPTORPOOLBUILDER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <inttypes.h>
#include <stdexcept>

#include "Logger.h"

#include "VulkanDeviceManager.h"

class VulkanDescriptorPoolBuilder
{
public:
    VulkanDescriptorPoolBuilder() = default;
    ~VulkanDescriptorPoolBuilder() = default;
    VulkanDescriptorPoolBuilder(const VulkanDescriptorPoolBuilder&) = delete;
    VulkanDescriptorPoolBuilder& operator=(VulkanDescriptorPoolBuilder&) = delete;
    VulkanDescriptorPoolBuilder(const VulkanDescriptorPoolBuilder&&) noexcept = delete;
    VulkanDescriptorPoolBuilder& operator=(VulkanDescriptorPoolBuilder&&) noexcept = delete;
public:
    
    void Reset()
    {
        maxSets = 0;
        allowFreeDescriptorSet = false;
        poolSizes.clear();
    }

    VkDescriptorPool Build()
    {
        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = maxSets;
        
        if (allowFreeDescriptorSet)
            poolInfo.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        VkDescriptorPool pool{VK_NULL_HANDLE};
        VkResult result = vkCreateDescriptorPool(GetVulkanDeviceManager().GetDeviceChecked(), &poolInfo, nullptr, &pool);
        if (result != VK_SUCCESS)
        {
            Log(Error, "Vulkan", "Failed to build descriptor pool, result code : %d", result);
            throw std::runtime_error("Failed to build descriptor pool");
        }
        return pool;
    }

    VulkanDescriptorPoolBuilder& AddUniformBuffer(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count);}

    VulkanDescriptorPoolBuilder& AddUniformBufferDynamic(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, count);}

    VulkanDescriptorPoolBuilder& AddStorageBuffer(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, count);}

    VulkanDescriptorPoolBuilder& AddStorageBufferDynamic(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, count);}

    VulkanDescriptorPoolBuilder& AddSampledImage(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count); }

    VulkanDescriptorPoolBuilder& AddCombinedImageSampler(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, count); }

    VulkanDescriptorPoolBuilder& AddSampler(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_SAMPLER, count); }

    VulkanDescriptorPoolBuilder& AddStorageImage(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count); }

    VulkanDescriptorPoolBuilder& AddInputAttachment(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, count); }

    VulkanDescriptorPoolBuilder& AddInlineUniformBlock(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK, count); }

    VulkanDescriptorPoolBuilder& Add(VkDescriptorType type, uint32_t count)
    {
        for (auto& poolSize : poolSizes)
        {
            if (poolSize.type == type) 
            { 
                Log(Warning, "Vulkan", "Descriptor pool builder, multiple pool size add");
                poolSize.descriptorCount = count; 
                return * this; 
            }
        }

        poolSizes.push_back({
            .type = type,
            .descriptorCount = count
        });

        return *this;
    }

    VulkanDescriptorPoolBuilder& SetMaxSets(uint32_t num) 
    { 
        maxSets = num;
        return *this; 
    }

    VulkanDescriptorPoolBuilder& AllowFreeDescriptorSet() 
    { 
        allowFreeDescriptorSet = true;
        return *this; 
    }

private:
    bool allowFreeDescriptorSet{false};
    uint32_t maxSets;
    std::vector<VkDescriptorPoolSize> poolSizes;
};

#endif // VULKANDESCRIPTORPOOLBUILDER_H