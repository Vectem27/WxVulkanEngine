#include "VulkanDescriptorUtils.h"

#include <stdexcept>

#include "Logger.h"
#include "VulkanDeviceManager.h"

VkDescriptorSet VulkanDescriptorUtils::AllocateSet(VkDescriptorPool pool, VkDescriptorSetLayout setLayouts)
{
    if (pool == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to allocate descriptor set, descriptor pool is null");
        throw std::invalid_argument("Failed to allocate descriptor set, descriptor pool is null");
    }

    if (pool == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to allocate descriptor set, descriptor set layout is null");
        throw std::invalid_argument("Failed to allocate descriptor set, descriptor set layout is null");
    }

    VkDescriptorSet descSet;

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &setLayouts;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = pool;

    if(auto result = vkAllocateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), &allocInfo, &descSet); result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to allocate descriptor set. Result code : %d", result);
        throw std::runtime_error("Failed to allocate descriptor set");
    }

    return descSet;
}
