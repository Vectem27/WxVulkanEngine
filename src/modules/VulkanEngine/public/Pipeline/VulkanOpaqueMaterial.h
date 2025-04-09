#ifndef VULKANOPAQUEMATERIAL_H
#define VULKANOPAQUEMATERIAL_H

#include "IVulkanMaterial.h"

class VulkanPipelineManager;

class VulkanOpaqueMaterial : public IVulkanMaterial
{
public:
    VulkanOpaqueMaterial(VulkanPipelineManager* pipelineManager) : pipelineManager(pipelineManager) {}
    virtual void CreatePipelines(VkDevice device, VkRenderPass renderPass, MaterialInfo materialInfo) override;
    void CreateShadowMapPipeline(VkDevice device, VkRenderPass renderPass, MaterialInfo materialInfo);
    virtual VkPipeline GetBasePipeline() const override { return basePipeline; }
    virtual VkPipeline GetShadowMapPipeline() const { return shadowMapPipeline; };
    virtual void Bind(VkCommandBuffer commandBuffer, ERenderPassType pass) const override;

private:
    VulkanPipelineManager* pipelineManager{nullptr};
    VkPipeline basePipeline{VK_NULL_HANDLE};
    VkPipeline shadowMapPipeline{VK_NULL_HANDLE};
};

#endif // VULKANOPAQUEMATERIAL_H