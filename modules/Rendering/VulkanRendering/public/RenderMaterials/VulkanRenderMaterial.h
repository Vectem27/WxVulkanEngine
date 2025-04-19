#ifndef VULKANRENDERMATERIAL_H
#define VULKANRENDERMATERIAL_H

#include "IRenderMaterial.h"

namespace Vulkan
{
    class VulkanRenderMaterial : public RHI::IRenderMaterial
    {
    public:
        VulkanRenderMaterial();
        virtual ~VulkanRenderMaterial();

    public: // IRenderMaterial Interface
        virtual void SetInteger(int value, uint32_t parameterIndex) override;
        virtual void SetScalar(float value, uint32_t parameterIndex) override;
        virtual void SetVector(Vector4f vector, uint32_t parameterIndex) override;
        virtual void SetTexture(RHI::IRenderTexture* texture, uint32_t parameterIndex) override;

    private:

    };
}

#endif // VULKANRENDERMATERIAL_H