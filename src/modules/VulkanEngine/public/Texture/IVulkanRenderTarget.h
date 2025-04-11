#ifndef IVULKANRENDERTARGET_H
#define IVULKANRENDERTARGET_H

#include <vulkan/vulkan.h>

/**
 * @interface IVulkanRenderTarget
 */
class IVulkanRenderTarget
{
public:
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void StartRendering() = 0;
    virtual VkFramebuffer GetGeometryFrameBuffer() const = 0;
    virtual VkFramebuffer GetLightingFrameBuffer() const = 0;
    virtual VkFramebuffer GetPostprocessFrameBuffer() const = 0;
};

#endif // IVULKANRENDERTARGET_H