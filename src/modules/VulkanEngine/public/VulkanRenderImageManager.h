#ifndef VULKANRENDERIMAGEMANAGER_H
#define VULKANRENDERIMAGEMANAGER_H

#include <vulkan/vulkan.h>
#include <inttypes.h>

class VulkanRenderImageManager
{
private:
    VulkanRenderImageManager() = default;
public:
    static VulkanRenderImageManager* GetInstance();
    void Init(VkDevice device, VkPhysicalDevice physicalDevice);

    void CreateImage(VkImage &image, VkDeviceMemory& imageMemory, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usageFlags) const;
    void CreateImageView(VkImageView &imageView, VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags) const;

    void CreateColorImage(VkImage &depthImage, VkDeviceMemory& depthImageMemory, uint32_t width, uint32_t height) const;
    void CreateColorImageView(VkImageView &depthImageView, VkImage &depthImage) const;

    void CreateDepthStencilImage(VkImage &depthImage, VkDeviceMemory& depthImageMemory, uint32_t width, uint32_t height) const;
    void CreateDepthStencilImageView(VkImageView &depthImageView, VkImage &depthImage) const;
private:
    VkDevice device{VK_NULL_HANDLE};
    VkPhysicalDevice physicalDevice{VK_NULL_HANDLE};
    static VulkanRenderImageManager* instance;

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
};

VulkanRenderImageManager& GetVulkanImageManager();

#endif // VULKANRENDERIMAGEMANAGER_H