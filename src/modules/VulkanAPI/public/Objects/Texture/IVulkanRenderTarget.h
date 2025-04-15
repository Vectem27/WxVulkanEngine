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

    virtual VkFramebuffer GetGeometryFrameBuffer() const = 0;
    virtual VkFramebuffer GetLightingFrameBuffer() const = 0;
    virtual VkFramebuffer GetPostprocessFrameBuffer() const = 0;

    virtual void BeginRendering(VkCommandBuffer commandBuffer) = 0;
    virtual void EndRendering(VkQueue queue, VkCommandBuffer commandBuffer) = 0;
    virtual void StartLighting(VkCommandBuffer commandBuffer) {}
    virtual void StartPostprocessing(VkCommandBuffer commandBuffer) {}
};

#endif // IVULKANRENDERTARGET_H