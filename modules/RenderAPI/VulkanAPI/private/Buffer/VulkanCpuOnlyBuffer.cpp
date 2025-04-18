#include "VulkanCpuOnlyBuffer.h"

#include "Logger.h"
#include "VulkanDeviceManager.h"

void VulkanCpuOnlyBuffer::Update(const void *data, VkDeviceSize offset, size_t size)
{
    if (bufferMemory == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to update buffer, bufferMemory is null");
        return;
    }

    if (!data) 
    {
        Log(Error, "Vulkan", "Failed to update buffer, data is null");
        return;
    }

    if (offset + size > bufferSize) 
    {
        Log(Error, "Vulkan", "Failed to update buffer : offset + size exceeds buffer size");
        return;
    }

    try
    {
        void* mappedData;
        vkMapMemory(GetVulkanDeviceManager().GetDeviceChecked(), bufferMemory, offset, size, 0, &mappedData);
        memcpy(mappedData, data, size);
        vkUnmapMemory(GetVulkanDeviceManager().GetDeviceChecked(), bufferMemory);
    }
    catch(...)
    {
        Log(Error, "Vulkan", "Failed to update buffer memory");
    }
}