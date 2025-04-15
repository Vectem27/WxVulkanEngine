#ifndef VULKANSHADOWMAP_H
#define VULKANSHADOWMAP_H

#include <vulkan/vulkan.h>
#include "IVulkanShadowMap.h"

class VulkanTexture;

class VulkanShadowMap : public IVulkanShadowMap
{
public:
    VulkanShadowMap(uint32_t resolution = 128);
    virtual ~VulkanShadowMap();

public: // IVulkanShadowMap Interface
    virtual uint32_t GetResolution() const override { return resolution; }
    virtual VkFramebuffer GetFramebuffer() const override { return frameBuffer; }

    virtual void BeginRendering(VkCommandBuffer commandBuffer) override;
    virtual void EndRendering(VkQueue queue, VkCommandBuffer commandBuffer) override;
public:
    VkImageView GetImageView() const;
    VkImageView& GetImageView();

private:
    uint32_t resolution{128};

    VkFramebuffer frameBuffer{VK_NULL_HANDLE};

    VulkanTexture* depthTexture{nullptr};
};

#endif // VULKANSHADOWMAP_H