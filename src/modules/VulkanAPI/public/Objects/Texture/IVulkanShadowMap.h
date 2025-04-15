#ifndef IVULKANSHADOWMAP_H
#define IVULKANSHADOWMAP_H

#include <inttypes.h>

class IVulkanShadowMap
{
public:
    virtual ~IVulkanShadowMap() = default;

    virtual uint32_t GetResolution() const = 0;
    virtual VkFramebuffer GetFramebuffer() const = 0;

    virtual void BeginRendering(VkCommandBuffer commandBuffer) = 0;
    virtual void EndRendering(VkQueue queue, VkCommandBuffer commandBuffer) = 0;
};

#endif // IVULKANSHADOWMAP_H