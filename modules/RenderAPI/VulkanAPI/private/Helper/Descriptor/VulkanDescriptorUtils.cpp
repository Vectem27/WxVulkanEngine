#include "VulkanDescriptorUtils.h"

#include <stdexcept>

#include "Logger.h"
#include "VulkanDeviceManager.h"
#include "VulkanBuffer.h"

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

void VulkanDescriptorUtils::UpdateSet(VkDescriptorSet descriptorSet, const VulkanBuffer& buffer, VkDescriptorType type, uint32_t binding) noexcept
{
    if (descriptorSet == VK_NULL_HANDLE)
    {
        Log(Warning, "Vulkan", "Failed to update descriptor set, descriptor set is null");
    }

    if (buffer.GetBuffer() == VK_NULL_HANDLE)
    {
        Log(Warning, "Vulkan", "Failed to update descriptor set, buffer is null");
    }

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = buffer.GetBufferSize();

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = type;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), 1, &descriptorWrite, 0, nullptr);
}

void VulkanDescriptorUtils::FreeDescriptorSet(VkDescriptorPool descriptorPool, VkDescriptorSet &descriptorSet) noexcept
{
    if (descriptorSet == VK_NULL_HANDLE)
        return;

    if (descriptorPool == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to free descriptor set, descriptor pool is null");
    }

    try
    {
        if (auto result = vkFreeDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), descriptorPool, 1, &descriptorSet); result != VK_SUCCESS)
        {
            Log(Error, "Vulkan", "Failed to free descriptor set. Result code : %d", result);
        }
    }
    catch(...)
    {
        Log(Error, "Vulkan", "Failed to free descriptor set");
    }

    descriptorSet = VK_NULL_HANDLE;
}

void VulkanDescriptorUtils::DestroyDescriptorPool(VkDescriptorPool& descriptorPool) noexcept
{
    if (descriptorPool == VK_NULL_HANDLE)
        return;

    try
    {
        vkDestroyDescriptorPool(GetVulkanDeviceManager().GetDeviceChecked(), descriptorPool, nullptr);
    }
    catch(...)
    {
        Log(Error, "Vulkan", "Failed to destroy descriptor pool");
    }

    descriptorPool = VK_NULL_HANDLE;
}
