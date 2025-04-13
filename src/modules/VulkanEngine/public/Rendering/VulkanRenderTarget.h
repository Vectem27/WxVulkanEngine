#ifndef VULKANRENDERTARGET_H
#define VULKANRENDERTARGET_H

#include <vulkan/vulkan.h>

#include "IVulkanRenderTarget.h"
#include "VulkanTexture.h"

class VulkanRenderTarget : public IVulkanRenderTarget
{
public:
    VulkanRenderTarget() = default;
    virtual ~VulkanRenderTarget() {Cleanup();}
public:
    void Init(uint32_t width, uint32_t height);
    void Cleanup() noexcept;
    void CleanupFrameBuffer() noexcept;

public: // IVulkanRenderTarget Interface
    virtual uint32_t GetWidth() const override { return width; }
    virtual uint32_t GetHeight() const override { return height; }

    virtual void BeginRendering(VkCommandBuffer commandBuffer) override;
    virtual void EndRendering(VkQueue queue, VkCommandBuffer commandBuffer) override;

public: // GBuffer
    VkFramebuffer& GetGeometryFrameBuffer() { return geometryFrameBuffer; }
    VkFramebuffer GetGeometryFrameBuffer() const { return geometryFrameBuffer; }

public: // Lighting
    VkFramebuffer& GetLightingFrameBuffer() { return lightingFrameBuffer; }
    VkFramebuffer GetLightingFrameBuffer() const { return lightingFrameBuffer; }

public: // Post-process
    VkFramebuffer& GetPostprocessFrameBuffer() { return postprocessFrameBuffer; }
    VkFramebuffer GetPostprocessFrameBuffer() const { return postprocessFrameBuffer; }

public:
    VkImageView& GetBaseColorImageView() { return baseColorTexture.GetImageView(); }
    VkImageView& GetDepthStencilImageView() { return depthStencilTexture.GetImageView(); }
    VkImageView& GetPositionImageView() { return positionTexture.GetImageView(); }
    VkImageView& GetNormalImageView() { return normalTexture.GetImageView(); }
    VkImageView& GetLightingImageView() { return lightingTexture.GetImageView(); }
    VkImageView& GetPostprocessImageView() { return postprocessTexture.GetImageView(); }
public:
    void CreateFrameBuffer();
private:
    uint32_t width{128}, height{128};

    VulkanTexture baseColorTexture;
    VulkanTexture depthStencilTexture;
    VulkanTexture positionTexture;
    VulkanTexture normalTexture;
    VulkanTexture lightingTexture;
    VulkanTexture postprocessTexture;

    VkFramebuffer geometryFrameBuffer{VK_NULL_HANDLE};
    VkFramebuffer lightingFrameBuffer{VK_NULL_HANDLE};
    VkFramebuffer postprocessFrameBuffer{VK_NULL_HANDLE};
};

#endif // VULKANRENDERTARGET_H