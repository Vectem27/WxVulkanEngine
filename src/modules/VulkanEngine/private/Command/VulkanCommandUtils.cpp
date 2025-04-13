#include "VulkanCommandUtils.h"

#include "Logger.h"
#include "VulkanDeviceManager.h"

VkCommandBuffer VulkanCommandUtils::AllocateCommandBuffer(VkCommandPool commandPool, VkCommandBufferLevel level)
{
    if (commandPool == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to allocate commande buffer : command pool is null");
        throw std::invalid_argument("Failed to allocate commande buffer : command pool is null");
    }

    auto device = GetVulkanDeviceManager().GetDeviceChecked();

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (auto result = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer); result != VK_SUCCESS) 
    {
        Log(Error, "Vulkan", "Failed to allocate command buffer, result code : %d", result);
        throw std::runtime_error("Failed to allocate command buffer");
    }

    return commandBuffer;
}

void VulkanCommandUtils::ResetCommandBuffer(VkCommandBuffer commandBuffer)
{
    if (commandBuffer == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to reset command buffer : command buffer is null");
        throw std::invalid_argument("Failed to reset command buffer : command buffer is null");
    }

    if (auto result = vkResetCommandBuffer(commandBuffer, 0); result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to reset command buffer, result code : %d", result);
        throw std::runtime_error("Failed to reset command buffer");
    }
}


void VulkanCommandUtils::BeginCommandBuffer(VkCommandBuffer commandBuffer)
{
    if (commandBuffer == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to begin recording command buffer : command buffer is null");
        throw std::invalid_argument("Failed to begin recording command buffer : command buffer is null");
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;  // Permet la réutilisation avant fin
    beginInfo.pInheritanceInfo = nullptr;  // Pas d'info d'héritage pour un command buffer primaire

    if (auto result = vkBeginCommandBuffer(commandBuffer, &beginInfo); result != VK_SUCCESS) 
    {
        Log(Error, "Vulkan", "Failed to begin recording command buffer, result code : %d", result);
        throw std::runtime_error("Failed to begin recording command buffer");
    }
}

void VulkanCommandUtils::EndCommandBuffer(VkCommandBuffer commandBuffer)
{
    if (commandBuffer == VK_NULL_HANDLE) 
    {
        Log(Error, "Vulkan", "Failed to record command buffer : command buffer is null");
        throw std::invalid_argument("Failed to record command buffer : command buffer is null");
    }

    if (auto result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS) 
    {
        Log(Error, "Vulkan", "Failed to record command buffer, result code : %d", result);
        throw std::runtime_error("Failed to record command buffer");
    }
}

void VulkanCommandUtils::SubmitCommandBuffer(VkQueue queue, const VkCommandBuffer commandBuffer)
{
    if (queue == VK_NULL_HANDLE) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer : queue is null");
        throw std::invalid_argument("Failed to submit command buffer : queue is null");
    }

    if (commandBuffer == VK_NULL_HANDLE) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer : command buffer is null");
        throw std::invalid_argument("Failed to submit command buffer : command buffer is null");
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    
    if (auto result = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE); result != VK_SUCCESS) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer, result code : %d", result);
        throw std::runtime_error("Failed to submit command buffer");
    }
}

void VulkanCommandUtils::SubmitCommandBufferWithSync(VkQueue queue, const VkCommandBuffer commandBuffer, VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, VkFence fence)
{
    if (queue == VK_NULL_HANDLE) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer with sync : queue is null");
        throw std::invalid_argument("Failed to submit command buffer with sync : queue is null");
    }

    if (commandBuffer == VK_NULL_HANDLE) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer with sync : command buffer is null");
        throw std::invalid_argument("Failed to submit command buffer with sync : command buffer is null");
    }

    if (waitSemaphore == VK_NULL_HANDLE) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer with sync : wait semaphore is null");
        throw std::invalid_argument("Failed to submit command buffer with sync : wait semaphore is null");
    }

    if (signalSemaphore == VK_NULL_HANDLE) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer with sync : signal semaphore is null");
        throw std::invalid_argument("Failed to submit command buffer with sync : signal semaphore is null");
    }

    if (fence == VK_NULL_HANDLE) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer with sync : fence is null");
        throw std::invalid_argument("Failed to submit command buffer with sync : fence is null");
    }

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &signalSemaphore;

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &waitSemaphore;
    submitInfo.pWaitDstStageMask = waitStages;

    if (auto result = vkQueueSubmit(queue, 1, &submitInfo, fence); result != VK_SUCCESS) 
    {
        Log(Error, "Vulkan", "Failed to submit command buffer with sync, result code : %d", result);
        throw std::runtime_error("Failed to submit command buffer with sync");
    }
}

void VulkanCommandUtils::FreeCommandBuffer(VkCommandPool commandPool, VkCommandBuffer commandBuffer) noexcept
{
    try
    {
        if (commandPool == VK_NULL_HANDLE)
        {
            Log(Warning, "Vulkan", "Failed to free command buffer : command pool is null");
            return;
        }

        if (commandBuffer != VK_NULL_HANDLE)
        {
            vkFreeCommandBuffers(GetVulkanDeviceManager().GetDeviceChecked(), commandPool, 1, &commandBuffer);
        }        
    }
    catch(...)
    {
        Log(Warning, "Vulkan", "Failed to free command buffer");
    }
}

void VulkanCommandUtils::DestroyCommandPool(VkCommandPool commandPool) noexcept
{
    try
    {
        if (commandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(GetVulkanDeviceManager().GetDeviceChecked(), commandPool, nullptr);
        }
    }
    catch(...)
    {
        Log(Warning, "Vulkan", "Failed to destroy command pool");
    }
}
