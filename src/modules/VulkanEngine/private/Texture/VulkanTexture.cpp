#include "VulkanTexture.h"

#include "Logger.h"
#include "VulkanDeviceManager.h"
#include "VulkanImageUtils.h"

void VulkanTexture::InitTexture(
    uint32_t width, uint32_t height, VkFormat format, 
    VkImageUsageFlags usageFlags, VkImageAspectFlags aspectFlags)
{
    this->format = format;
    this->width = width;
    this->height = height;

    Cleanup();

    VulkanImageUtils::CreateImage(
        image,
        imageMemory,
        width,
        height,
        format,
        usageFlags
    );

    VulkanImageUtils::CreateImageView(
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
        auto device = GetVulkanDeviceManager().GetDeviceChecked();

        if (imageView != VK_NULL_HANDLE)
            vkDestroyImageView(device, imageView, nullptr);
        if (image != VK_NULL_HANDLE)
            vkDestroyImage(device, image, nullptr);
        if (imageMemory != VK_NULL_HANDLE)
            vkFreeMemory(device, imageMemory, nullptr);
    }
    catch(...)
    {
        Log(Warning, "Vulkan", "Failed to cleanup texture");
    }   
    
    image = VK_NULL_HANDLE;
    imageMemory = VK_NULL_HANDLE;
    imageView = VK_NULL_HANDLE;
}
