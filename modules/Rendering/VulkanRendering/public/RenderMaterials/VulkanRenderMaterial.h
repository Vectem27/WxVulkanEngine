#ifndef VULKANRENDERMATERIAL_H
#define VULKANRENDERMATERIAL_H

// Libraries
#include <vulkan/vulkan.h>


//RHI
#include "IRenderMaterial.h"

class VulkanPipelineInfo;

namespace Vulkan
{
    class VulkanRenderMaterial : public RHI::IRenderMaterial
    {
    public:
        // TODO: Complete these functions
        VulkanRenderMaterial() = default;
        virtual ~VulkanRenderMaterial() = default;

        virtual void InitRenderPipeline(const VulkanPipelineInfo& pipelineInfo) = 0;
    public: // IRenderMaterial Interface
        // TODO: Complete these functions
        virtual void SetInteger(int value, uint32_t parameterIndex) override {}
        virtual void SetScalar(float value, uint32_t parameterIndex) override {}
        virtual void SetVector(Vector4f vector, uint32_t parameterIndex) override {}
        virtual void SetTexture(RHI::IRenderTexture* texture, uint32_t parameterIndex) override {}

    public:
        void BindRenderMaterial(VkCommandBuffer commandBuffer) const;

    protected:
        VkPipeline renderPipeline{ VK_NULL_HANDLE };
    };
}

#endif // VULKANRENDERMATERIAL_H