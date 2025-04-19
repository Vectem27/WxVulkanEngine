#ifndef VULKANSURFACERENDERMATERIAL_H
#define VULKANSURFACERENDERMATERIAL_H

#include "VulkanRenderMaterial.h"

namespace Vulkan 
{
    class SurfaceRenderMaterial : public VulkanRenderMaterial
    {
    public:
        SurfaceRenderMaterial();
        virtual ~SurfaceRenderMaterial();
    };
} // namespace Vulkan

#endif // VULKANSURFACERENDERMATERIAL_H