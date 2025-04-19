#ifndef IVULKANMATERIAL_H
#define IVULKANMATERIAL_H

#include <string>

#include "VulkanPipelineInfo.h"

struct MaterialInfo
{
    std::string vertexShader = "";
    std::string fragmentShader = "";
    std::string shadowMapVertexShader = "";

    std::string lightingVertexShader = "";
    std::string lightingFragmentShader = "";
};


class IVulkanMaterial
{
public:
    // TODO: Remove these getters
    virtual VkPipeline GetBasePipeline() const = 0;
    virtual VkPipeline GetShadowMapPipeline() const = 0;

    virtual void Bind(VkCommandBuffer commandBuffer) const = 0;
};

#endif // IVULKANMATERIAL_H