#ifndef VULKANRENDERIMAGEMANAGER_H
#define VULKANRENDERIMAGEMANAGER_H

#include <vulkan/vulkan.h>
#include <inttypes.h>

namespace VulkanImageUtils
{
    /**
     * @brief Create a Vulkan image and allocate memory for it.
     * @param image The created Vulkan image.
     * @param imageMemory The allocated memory for the image.
     * @param width The width of the image in pixels.
     * @param height The height of the image in pixels.
     * @param format The image format (e.g., VK_FORMAT_R8G8B8A8_SRGB).
     * @param usageFlags The usage flags for the image (e.g., VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT).
     */
    void CreateImage(VkImage &image, VkDeviceMemory& imageMemory, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usageFlags);

    /**
     * @brief Create a Vulkan image view for the given image.
     * @param imageView The created image view.
     * @param image The Vulkan image for which to create the view.
     * @param format The format of the image.
     * @param aspectFlags The aspect flags for the image view (e.g., VK_IMAGE_ASPECT_COLOR_BIT).
     */
    void CreateImageView(VkImageView &imageView, VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags);
};

#endif // VULKANRENDERIMAGEMANAGER_H