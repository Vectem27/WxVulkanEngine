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
        poolSizes.clear();
    }

    VkDescriptorPool Build()
    {
        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = maxSets;

        VkDescriptorPool pool;
        vkCreateDescriptorPool(GetVulkanDeviceManager().GetDeviceChecked(), &poolInfo, nullptr, &pool);
        return pool;
    }

    VulkanDescriptorPoolBuilder& AddUniformBuffer(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count); return *this; }

    VulkanDescriptorPoolBuilder& AddUniformBufferDynamic(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, count); return *this; }

    VulkanDescriptorPoolBuilder& AddStorageBuffer(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, count); return *this; }

    VulkanDescriptorPoolBuilder& AddStorageBufferDynamic(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, count); return *this; }

    VulkanDescriptorPoolBuilder& AddSampledImage(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count); return *this; }

    VulkanDescriptorPoolBuilder& AddCombinedImageSampler(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, count); return *this; }

    VulkanDescriptorPoolBuilder& AddSampler(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_SAMPLER, count); return *this; }

    VulkanDescriptorPoolBuilder& AddStorageImage(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count); return *this; }

    VulkanDescriptorPoolBuilder& AddInputAttachment(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, count); return *this; }

    VulkanDescriptorPoolBuilder& AddInlineUniformBlock(uint32_t count) 
    { Add(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK, count); return *this; }

    void Add(VkDescriptorType type, uint32_t count)
    {
        for (auto& poolSize : poolSizes)
        {
            if (poolSize.type == type) 
            { 
                Log(Warning, "Vulkan", "Descriptor pool builder, multiple pool size add");
                poolSize.descriptorCount = count; 
                return; 
            }
        }

        poolSizes.push_back({
            .type = type,
            .descriptorCount = count
        });
    }

private:
    uint32_t maxSets;
    std::vector<VkDescriptorPoolSize> poolSizes;
};

#endif // VULKANDESCRIPTORPOOLBUILDER_H