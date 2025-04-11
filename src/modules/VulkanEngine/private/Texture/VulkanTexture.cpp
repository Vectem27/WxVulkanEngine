#include "VulkanTexture.h"

#include "Logger.h"
#include "VulkanDeviceManager.h"
#include "VulkanRenderImageManager.h"

void VulkanTexture::InitTexture(
    uint32_t width, uint32_t height, VkFormat format, 
    VkImageUsageFlags usageFlags, VkImageAspectFlags aspectFlags)
{
    this->format = format;
    this->width = width;
    this->height = height;

    Cleanup();

    GetVulkanImageManager().CreateImage(
        image,
        imageMemory,
        width,
        height,
        format,
        usageFlags
    );

    GetVulkanImageManager().CreateImageView(
        imageView,
        image,
        format,
        aspectFlags
    );
}

void VulkanTexture::Cleanup() noexcept
{
    try
    {
        if (image != VK_NULL_HANDLE)
        vkDestroyImage(GetVulkanDeviceManager().GetDeviceChecked(), image, nullptr);
        if (imageMemory != VK_NULL_HANDLE)
            vkFreeMemory(GetVulkanDeviceManager().GetDeviceChecked(), imageMemory, nullptr);
        if (imageView != VK_NULL_HANDLE)
            vkDestroyImageView(GetVulkanDeviceManager().GetDeviceChecked(), imageView, nullptr);
    }
    catch(...)
    {
        Log(Warning, "Vulkan", "Failed to cleanup texture");
    }
    
    
    image = VK_NULL_HANDLE;
    imageMemory = VK_NULL_HANDLE;
    imageView = VK_NULL_HANDLE;
}
