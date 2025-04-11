#ifndef VULKANSHADOWMAPRENDERTARGET_H
#define VULKANSHADOWMAPRENDERTARGET_H

#include <vulkan/vulkan.h>
#include "IRenderTarget.h"
#include "VulkanTexture.h"

class VulkanShadowMapRenderTarget : public IRenderTarget
{
public:
    VulkanShadowMapRenderTarget(class VulkanRenderEngine* renderEngine, uint32_t width, uint32_t height, VkFormat format);
    ~VulkanShadowMapRenderTarget();

    VkFramebuffer GetFramebuffer() const { return framebuffer; }
    VkImageView GetImageView() const { return depthTexture.GetImageView(); }
    VkImageView& GetImageView() { return depthTexture.GetImageView(); }
    const VkCommandBuffer& GetCommandBuffer() const { return commandBuffer; }
    const VkCommandBuffer& GetTransferCommandBuffer() const { return transferCommandBuffer; }

    void CreateFramebuffer(VkRenderPass renderPass);

    virtual uint32_t GetWidth() const override { return width; }
    virtual uint32_t GetHeight() const override { return height; }

    //void CopyToCpuBuffer(void* dst, unsigned long size);
private:
    void CreateCommandPool(uint32_t graphicsQueueFamilyIndex);
    void CreateCommandBuffer();  
    //void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
private:
    class VulkanRenderEngine* renderEngine;
    
    uint32_t width, height;
    VkFormat format;

    VkFramebuffer framebuffer{VK_NULL_HANDLE};

    VkCommandPool commandPool{VK_NULL_HANDLE};
    VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
    VkCommandBuffer transferCommandBuffer{VK_NULL_HANDLE};

    VulkanTexture depthTexture;
};

#endif // VULKANSHADOWMAPRENDERTARGET_H