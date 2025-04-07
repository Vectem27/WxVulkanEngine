#ifndef IVULKANMATERIAL_H
#define IVULKANMATERIAL_H

#include <vulkan/vulkan.h>
#include <string>
#include "EngineCore.hpp"
#include "ERenderPassType.h"

struct MaterialInfo
{
    std::string vertexShader = "";
    std::string fragmentShader = "";
};

class IVulkanMaterial
{
public:
    virtual void CreatePipelines(VkDevice device, VkRenderPass renderPass, MaterialInfo materialInfo) = 0;
    virtual VkPipeline GetBasePipeline() const = 0;
    virtual VkPipeline GetShadowMapPipeline() const = 0;

    virtual void Bind(VkCommandBuffer commandBuffer, ERenderPassType pass) const = 0;
};

#endif // IVULKANMATERIAL_H