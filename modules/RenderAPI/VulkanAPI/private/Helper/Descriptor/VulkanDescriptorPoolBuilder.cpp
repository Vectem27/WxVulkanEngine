#include "VulkanDescriptorPoolBuilder.h"

#include <stdexcept>

#include "Logger.h"
#include "VulkanDeviceManager.h"

VulkanDescriptorPoolBuilder& VulkanDescriptorPoolBuilder::Reset()
{
    maxSets = 0;
    allowFreeDescriptorSet = false;
    poolSizes.clear();
    return *this;
}

VkDescriptorPool VulkanDescriptorPoolBuilder::Build()
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

VulkanDescriptorPoolBuilder &VulkanDescriptorPoolBuilder::Add(VkDescriptorType type, uint32_t count)
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

VulkanDescriptorPoolBuilder &VulkanDescriptorPoolBuilder::SetMaxSets(uint32_t num)
{ 
    maxSets = num;
    return *this; 
}

VulkanDescriptorPoolBuilder &VulkanDescriptorPoolBuilder::AllowFreeDescriptorSet()
{ 
    allowFreeDescriptorSet = true;
    return *this; 
}