#ifndef VULKANRENDERIMAGEMANAGER_H
#define VULKANRENDERIMAGEMANAGER_H

#include <vulkan/vulkan.h>
#include <inttypes.h>

class VulkanRenderImageManager
{
private:
    // Singleton pattern to ensure only one instance of VulkanRenderImageManager exists
    VulkanRenderImageManager() = default;
    VulkanRenderImageManager(const VulkanRenderImageManager&) = delete;
    VulkanRenderImageManager& operator=(const VulkanRenderImageManager&) = delete;
    VulkanRenderImageManager(VulkanRenderImageManager&&) = delete;
    VulkanRenderImageManager& operator=(VulkanRenderImageManager&&) = delete;
    virtual ~VulkanRenderImageManager() = default;

public:
    static VulkanRenderImageManager& GetInstance()
    {
        static VulkanRenderImageManager instance;
        return instance;
    }
    
    void CreateImage(VkImage &image, VkDeviceMemory& imageMemory, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usageFlags) const;
    void CreateImageView(VkImageView &imageView, VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags) const;

private:
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
};

inline VulkanRenderImageManager& GetVulkanImageManager()
{
    return VulkanRenderImageManager::GetInstance();
}

#endif // VULKANRENDERIMAGEMANAGER_H