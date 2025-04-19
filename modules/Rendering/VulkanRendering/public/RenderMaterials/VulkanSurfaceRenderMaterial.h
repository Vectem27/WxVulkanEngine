#ifndef VULKANSURFACERENDERMATERIAL_H
#define VULKANSURFACERENDERMATERIAL_H

#include "VulkanRenderMaterial.h"

namespace Vulkan 
{
    class SurfaceRenderMaterial : public VulkanRenderMaterial
    {
    public:
        SurfaceRenderMaterial() = default; // TODO: Implement constructor
        virtual ~SurfaceRenderMaterial() = default; // TODO: Implement destructor

        virtual void InitRenderPipeline(const VulkanPipelineInfo& pipelineInfo) override;
    };
} // namespace Vulkan

#endif // VULKANSURFACERENDERMATERIAL_H