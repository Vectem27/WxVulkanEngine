#ifndef VULKANRENDERTARGET_H
#define VULKANRENDERTARGET_H

#include <vulkan/vulkan.h>

#include "IVulkanRenderTarget.h"
#include "VulkanTexture.h"

class VulkanRenderTarget : public IVulkanRenderTarget
{
    VulkanRenderTarget() = default;
    virtual ~VulkanRenderTarget() {Cleanup();}
public:
    void Init(uint32_t width, uint32_t height);
    void Cleanup() noexcept;

public: // IVulkanRenderTarget Interface
    virtual void StartRendering() = 0;
    virtual uint32_t GetWidth() const override { return width; }
    virtual uint32_t GetHeight() const override { return height; }

public: // GBuffer
    VkFramebuffer& GetGeometryFrameBuffer() { return geometryFrameBuffer; }
    VkFramebuffer GetGeometryFrameBuffer() const { return geometryFrameBuffer; }

public: // Lighting
    VkFramebuffer& GetLightingFrameBuffer() { return lightingFrameBuffer; }
    VkFramebuffer GetLightingFrameBuffer() const { return lightingFrameBuffer; }

public: // Post-process
    VkFramebuffer& GetPostprocessFrameBuffer() { return postprocessFrameBuffer; }
    VkFramebuffer GetPostprocessFrameBuffer() const { return postprocessFrameBuffer; }
private:
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