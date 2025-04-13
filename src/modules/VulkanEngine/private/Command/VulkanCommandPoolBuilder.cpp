#include "VulkanCommandPoolBuilder.h"


#include <stdexcept>

#include "Logger.h"

#include "VulkanDeviceManager.h"

VulkanCommandPoolBuilder& VulkanCommandPoolBuilder::Reset()
{
    allowResetCommand = false;
    protec = false;
    transient = false;
    return *this;
}

VkCommandPool VulkanCommandPoolBuilder::Build()
{
    if (queueFamilyIndex == VK_QUEUE_FAMILY_IGNORED) 
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

VulkanCommandPoolBuilder& VulkanCommandPoolBuilder::SetQueueFamilyIndex(uint32_t queueFamilyIndex) 
{ 
    this->queueFamilyIndex = queueFamilyIndex;
    return *this; 
}

VulkanCommandPoolBuilder& VulkanCommandPoolBuilder::AllowResetCommandBuffer() 
{ 
    allowResetCommand = true;
    return *this; 
}

VulkanCommandPoolBuilder& VulkanCommandPoolBuilder::MarkTransient() 
{ 
    transient = true;
    return *this; 
}

VulkanCommandPoolBuilder& VulkanCommandPoolBuilder::MarkProtected()
{ 
    protec = true;
    return *this; 
}