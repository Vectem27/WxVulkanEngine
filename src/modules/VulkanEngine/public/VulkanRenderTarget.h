#ifndef VULKANRENDERTARGET_H
#define VULKANRENDERTARGET_H

#include <vulkan/vulkan.h>

class VulkanRenderTarget
{
public:
    VulkanRenderTarget(class VulkanRenderEngine* renderEngine, uint32_t width, uint32_t height, VkFormat format);
    ~VulkanRenderTarget();

    VkFramebuffer GetFramebuffer() const { return framebuffer; }
    VkImageView GetImageView() const { return imageView; }

    void CreateFramebuffer(VkRenderPass renderPass);

private:
    void CreateImage();
    void CreateImageView();
private:
    class VulkanRenderEngine* renderEngine;
    
    uint32_t width, height;
    VkFormat format;

    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkFramebuffer framebuffer;
};

#endif // VULKANRENDERTARGET_H