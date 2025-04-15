#ifndef VULKANOPAQUEMATERIAL_H
#define VULKANOPAQUEMATERIAL_H

#include "IVulkanSurfaceMaterial.h"

class VulkanPipelineManager;

class VulkanOpaqueMaterial : public IVulkanSurfaceMaterial
{
public:
    VulkanOpaqueMaterial(VulkanPipelineManager* pipelineManager) : pipelineManager(pipelineManager) {}
    void CreatePipelines(VkRenderPass renderPass, MaterialInfo materialInfo);
    void CreateShadowMapPipeline(VkRenderPass renderPass, MaterialInfo materialInfo);
    virtual VkPipeline GetBasePipeline() const override { return basePipeline; }
    virtual VkPipeline GetShadowMapPipeline() const { return shadowMapPipeline; };
    virtual void Bind(VkCommandBuffer commandBuffer, ERenderPassType pass) const override;
    virtual void Bind(VkCommandBuffer commandBuffer) const override;
    virtual void BindForShadowMap(VkCommandBuffer commandBuffer) const override;

private:
    VulkanPipelineManager* pipelineManager{nullptr};
    VkPipeline basePipeline{VK_NULL_HANDLE};
    VkPipeline shadowMapPipeline{VK_NULL_HANDLE};
};

#endif // VULKANOPAQUEMATERIAL_H