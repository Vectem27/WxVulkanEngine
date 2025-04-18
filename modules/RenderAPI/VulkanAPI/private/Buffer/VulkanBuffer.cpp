#include "VulkanBuffer.h"

#include <stdexcept>

#include "Logger.h"
#include "VulkanDeviceManager.h"

VulkanBuffer::VulkanBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties)
    : bufferUsage(usage), memoryProperties(memoryProperties)
{
}

VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept
    : buffer(other.buffer), bufferMemory(other.bufferMemory), bufferSize(other.bufferSize)
{
    other.buffer = VK_NULL_HANDLE;
    other.bufferMemory = VK_NULL_HANDLE;
    other.bufferSize = 0;
}

VulkanBuffer &VulkanBuffer::operator=(VulkanBuffer &&other) noexcept
{
    if (this != &other) {
        Cleanup();
        buffer = other.buffer;
        bufferMemory = other.bufferMemory;
        bufferSize = other.bufferSize;
        other.buffer = VK_NULL_HANDLE;
        other.bufferMemory = VK_NULL_HANDLE;
        other.bufferSize = 0;
    }
    return *this;
}

void VulkanBuffer::Create(VkDeviceSize size)
{
    bufferSize = size;

    // 1. Création du buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = bufferUsage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = vkCreateBuffer(GetVulkanDeviceManager().GetDeviceChecked(), &bufferInfo, nullptr, &buffer);
    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to create buffer, result code : %d", result);
        throw std::runtime_error("Failed to create uniform buffer");
    }                

    // 2. Allocation mémoire
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(GetVulkanDeviceManager().GetDeviceChecked(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = GetVulkanDeviceManager().FindMemoryType(
        memRequirements.memoryTypeBits, 
        memoryProperties
    );

    result = vkAllocateMemory(GetVulkanDeviceManager().GetDeviceChecked(), &allocInfo, nullptr, &bufferMemory);
    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to allocate buffer memory, result code : %d", result);
        throw std::runtime_error("Failed to allocate buffer memory");
    }  

    result = vkBindBufferMemory(GetVulkanDeviceManager().GetDeviceChecked(), buffer, bufferMemory, 0);
    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to bind buffer memory, result code : %d", result);
        throw std::runtime_error("Failed to bind buffer memory");
    }  

}

void VulkanBuffer::Cleanup() noexcept
{
    try
    {
        if (buffer != VK_NULL_HANDLE)
            vkDestroyBuffer(GetVulkanDeviceManager().GetDeviceChecked(), buffer, nullptr);

        if (bufferMemory != VK_NULL_HANDLE)
            vkFreeMemory(GetVulkanDeviceManager().GetDeviceChecked(), bufferMemory, nullptr);
    }
    catch(...)
    {
        Log(Warning, "Vulkan", "Failed to cleanup buffer");
    }

    buffer = VK_NULL_HANDLE;
    bufferMemory = VK_NULL_HANDLE;
}
