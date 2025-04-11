#ifndef VULKANTEXTURE_H
#define VULKANTEXTURE_H

#include <vulkan/vulkan.h>
#include "IVulkanTexture.h"

class VulkanTexture : public IVulkanTexture
{
public:
    VulkanTexture() = default;
    virtual ~VulkanTexture() { Cleanup(); }
public:
    void InitTexture(uint32_t width, uint32_t height, VkFormat format, 
        VkImageUsageFlags usageFlags, VkImageAspectFlags aspectFlags);

    void Cleanup() noexcept;    

    VkImageView GetImageView() const { return imageView; }
    VkImageView& GetImageView() { return imageView; }
    VkFormat GetFormat() const { return format; }
    virtual uint32_t GetWidth() const override { return width; }
    virtual uint32_t GetHeight() const override { return height; }

private:
    VkFormat format{VK_FORMAT_R8G8B8A8_UNORM};
    uint32_t width{128}, height{128};

    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
};

#endif // VULKANTEXTURE_H