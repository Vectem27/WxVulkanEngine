#ifndef VULKANCOMMANDUTILS_H
#define VULKANCOMMANDUTILS_H

#include <vulkan/vulkan.h>
#include "Logger.h"
#include "VulkanDeviceManager.h"

class VulkanCommandUtils
{
public:
    static VkCommandBuffer AllocateCommandBuffer(VkCommandPool commandPool, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) 
    {
        if (commandPool == VK_NULL_HANDLE)
        {
            Log(Error, "Vulkan", "Failed to allocate commande buffer : command pool is null");
            throw std::runtime_error("Failed to allocate commande buffer : command pool is null");
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

    static void ResetCommandBuffer(VkCommandBuffer commandBuffer)
    {
        if (commandBuffer == VK_NULL_HANDLE)
        {
            Log(Error, "Vulkan", "Failed to reset command buffer : command buffer is null");
            throw std::runtime_error("Failed to reset command buffer : command buffer is null");
        }

        if (auto result = vkResetCommandBuffer(commandBuffer, 0); result != VK_SUCCESS)
        {
            Log(Error, "Vulkan", "Failed to reset command buffer, result code : %d", result);
            throw std::runtime_error("Failed to reset command buffer");
        }
    }

    static void BeginCommandBuffer(VkCommandBuffer commandBuffer) 
    {
        if (commandBuffer == VK_NULL_HANDLE)
        {
            Log(Error, "Vulkan", "Failed to begin recording command buffer : command buffer is null");
            throw std::runtime_error("Failed to begin recording command buffer : command buffer is null");
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

    static void EndCommandBuffer(VkCommandBuffer commandBuffer) 
    {
        if (commandBuffer == VK_NULL_HANDLE) 
        {
            Log(Error, "Vulkan", "Failed to record command buffer : command buffer is null");
            throw std::runtime_error("Failed to record command buffer : command buffer is null");
        }

        if (auto result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS) 
        {
            Log(Error, "Vulkan", "Failed to record command buffer, result code : %d", result);
            throw std::runtime_error("Failed to record command buffer");
        }
    }

    static void SubmitCommandBuffer(VkQueue queue, VkCommandBuffer commandBuffer) 
    {
        if (queue == VK_NULL_HANDLE) 
        {
            Log(Error, "Vulkan", "Failed to submit command buffer : queue is null");
            throw std::runtime_error("Failed to submit command buffer : queue is null");
        }

        if (commandBuffer == VK_NULL_HANDLE) 
        {
            Log(Error, "Vulkan", "Failed to submit command buffer : command buffer is null");
            throw std::runtime_error("Failed to submit command buffer : command buffer is null");
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

    static void FreeCommandBuffer(VkCommandPool commandPool, VkCommandBuffer commandBuffer) noexcept
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

    static void DestroyCommandPool(VkCommandPool commandPool) noexcept
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
};

#endif // VULKANCOMMANDUTILS_H