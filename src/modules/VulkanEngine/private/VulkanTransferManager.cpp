#include "VulkanTransferManager.h"
#include "VulkanDeviceManager.h"
#include "Logger.h"

void VulkanTransferManager::InitTransfereManager()
{
    std::lock_guard<std::mutex> lock(mutex);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = GetVulkanDeviceManager().GetTransferQueueFamilyIndex();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    VkResult result = vkCreateCommandPool(GetVulkanDeviceManager().GetDeviceChecked(), &poolInfo, nullptr, &commandPool);
    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to initialize transfer manager, result code : %d", result);
        throw std::runtime_error("Failed to initialize transfer manager");
    }

    vkGetDeviceQueue(GetVulkanDeviceManager().GetDeviceChecked(), GetVulkanDeviceManager().GetTransferQueueFamilyIndex(), 0, &transferQueue);
    if (transferQueue == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to get transfere queue");
        throw std::runtime_error("Failed to get transfere queue");
    }
}

void VulkanTransferManager::Shutdown()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (commandPool == VK_NULL_HANDLE) 
        return;

    vkDestroyCommandPool(GetVulkanDeviceManager().GetDeviceChecked(), commandPool, nullptr);
}

void VulkanTransferManager::CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (commandPool == VK_NULL_HANDLE)
    {
        Log(Warning, "Vulkan", "Failed to copy buffer, command pool is null");
        throw std::runtime_error("Failed to copy buffer, command pool is null");
    }

    if (transferQueue == VK_NULL_HANDLE)
    {
        Log(Warning, "Vulkan", "Failed to copy buffer, transfer queue is null");
        throw std::runtime_error("Failed to copy buffer, transfer queue is null");
    }
    
    VkCommandBuffer cmdBuffer;
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    vkAllocateCommandBuffers(GetVulkanDeviceManager().GetDeviceChecked(), &allocInfo, &cmdBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmdBuffer, &beginInfo);

    VkBufferCopy copyRegion = {0, 0, size};
    vkCmdCopyBuffer(cmdBuffer, src, dst, 1, &copyRegion);

    vkEndCommandBuffer(cmdBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;

    VkFence fence;
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    vkCreateFence(GetVulkanDeviceManager().GetDeviceChecked(), &fenceInfo, nullptr, &fence);

    vkQueueSubmit(transferQueue, 1, &submitInfo, fence);
    vkWaitForFences(GetVulkanDeviceManager().GetDeviceChecked(), 1, &fence, VK_TRUE, UINT64_MAX);

    vkDestroyFence(GetVulkanDeviceManager().GetDeviceChecked(), fence, nullptr);
    vkFreeCommandBuffers(GetVulkanDeviceManager().GetDeviceChecked(), commandPool, 1, &cmdBuffer);
}
