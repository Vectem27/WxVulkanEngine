#ifndef VULKANRENDERTARGET_H
#define VULKANRENDERTARGET_H

#include <vulkan/vulkan.h>
#include "IRenderTarget.h"

class VulkanRenderTarget : public IRenderTarget
{
public:
    VulkanRenderTarget(class VulkanRenderEngine* renderEngine, uint32_t width, uint32_t height, VkFormat format);
    ~VulkanRenderTarget();

    VkFramebuffer GetFramebuffer() const { return framebuffer; }
    VkImageView GetImageView() const { return imageView; }
    VkRenderPass GetRenderPass() const {return renderPass; }
    const VkCommandBuffer& GetCommandBuffer() const { return commandBuffer; }
    const VkCommandBuffer& GetTransferCommandBuffer() const { return transferCommandBuffer; }

    void CreateFramebuffer(VkRenderPass renderPass);

    virtual uint32_t GetWidth() const override { return width; }
    virtual uint32_t GetHeight() const override { return height; }

    void CopyToCpuBuffer(void* dst, unsigned long size);
private:
    void CreateImage();
    void CreateImageView();
    void CreateCommandPool(uint32_t graphicsQueueFamilyIndex);
    void CreateCommandBuffer();  
    void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
private:
    class VulkanRenderEngine* renderEngine;
    
    uint32_t width, height;
    VkFormat format;

    VkImage image{VK_NULL_HANDLE};
    VkDeviceMemory imageMemory{VK_NULL_HANDLE};
    VkImageView imageView{VK_NULL_HANDLE};
    VkFramebuffer framebuffer{VK_NULL_HANDLE};

    VkRenderPass renderPass{VK_NULL_HANDLE};

    // Commands
    VkCommandPool commandPool{VK_NULL_HANDLE};
    VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
    VkCommandBuffer transferCommandBuffer{VK_NULL_HANDLE};
};

#endif // VULKANRENDERTARGET_H