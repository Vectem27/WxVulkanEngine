#ifndef VULKANCOMMANDPOOLBUILDER_H
#define VULKANCOMMANDPOOLBUILDER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <inttypes.h>
#include <stdexcept>

#include "Logger.h"

#include "VulkanDeviceManager.h"

class VulkanCommandPoolBuilder
{
public:
    VulkanCommandPoolBuilder() = default;
    ~VulkanCommandPoolBuilder() = default;
    VulkanCommandPoolBuilder(const VulkanCommandPoolBuilder&) = delete;
    VulkanCommandPoolBuilder& operator=(VulkanCommandPoolBuilder&) = delete;
    VulkanCommandPoolBuilder(const VulkanCommandPoolBuilder&&) noexcept = delete;
    VulkanCommandPoolBuilder& operator=(VulkanCommandPoolBuilder&&) noexcept = delete;
public:
    
    void Reset()
    {
        allowResetCommand = false;
        protec = false;
        transient = false;
    }

    VkCommandPool Build()
    {
        if ( queueFamilyIndex < 0) 
        {
            Log(Error, "Vulkan", "Failed to build command pool, invalid queue family index");
            throw std::runtime_error("Failed to build command pool, invalid queue family index");
        }


        VkCommandPoolCreateInfo poolCreateInfo = {};
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolCreateInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndex);
        poolCreateInfo.flags = 0;
        
        if (allowResetCommand)
            poolCreateInfo.flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; 
        if(transient)
            poolCreateInfo.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        if(protec)
            poolCreateInfo.flags |= VK_COMMAND_POOL_CREATE_PROTECTED_BIT;

        VkCommandPool commandPool{VK_NULL_HANDLE};
        VkResult result = vkCreateCommandPool(GetVulkanDeviceManager().GetDeviceChecked(), &poolCreateInfo, nullptr, &commandPool);
        if ( result != VK_SUCCESS) 
        {
            Log(Error, "Vulkan", "Failed to build command pool, result error : %d", result);
            throw std::runtime_error("Failed to build command pool");
        }
        return commandPool;
    }

    VulkanCommandPoolBuilder& AllowResetCommandBuffer() 
    { 
        allowResetCommand = true;
        return *this; 
    }

    VulkanCommandPoolBuilder& MarkTransient() 
    { 
        transient = true;
        return *this; 
    }

    VulkanCommandPoolBuilder& MarkProtected() 
    { 
        protec = true;
        return *this; 
    }
    
    VulkanCommandPoolBuilder& SetQueueFamilyIndex(uint32_t queueFamilyIndex) 
    { 
        this->queueFamilyIndex = static_cast<int64_t>(queueFamilyIndex);
        return *this; 
    }

private:
    bool allowResetCommand{false};
    bool transient{false};
    bool protec{false};
    int64_t queueFamilyIndex{-1};
};

#endif // VULKANCOMMANDPOOLBUILDER_H