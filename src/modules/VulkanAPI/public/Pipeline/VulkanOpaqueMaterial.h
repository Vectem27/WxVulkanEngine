#ifndef VULKANOPAQUEMATERIAL_H
#define VULKANOPAQUEMATERIAL_H

#include "IVulkanSurfaceMaterial.h"

class VulkanPipelineManager;

class VulkanOpaqueMaterial : public IVulkanSurfaceMaterial
{
public:
    void CreatePipelines(VulkanPipelineInfo pipelineInfo);
    void CreateShadowMapPipeline(VulkanPipelineInfo pipelineInf);
    virtual VkPipeline GetBasePipeline() const override { return basePipeline; }
    virtual VkPipeline GetShadowMapPipeline() const { return shadowMapPipeline; };
    virtual void Bind(VkCommandBuffer commandBuffer) const override;
    virtual void BindForShadowMap(VkCommandBuffer commandBuffer) const override;

private:
    VkPipeline basePipeline{VK_NULL_HANDLE};
    VkPipeline shadowMapPipeline{VK_NULL_HANDLE};
};

#endif // VULKANOPAQUEMATERIAL_H