#ifndef VULKANOPAQUEMATERIAL_H
#define VULKANOPAQUEMATERIAL_H

#include "IVulkanMaterial.h"

class VulkanPipelineManager;

class VulkanOpaqueMaterial : public IVulkanMaterial
{
public:
    VulkanOpaqueMaterial(VulkanPipelineManager* pipelineManager) : pipelineManager(pipelineManager) {}
    virtual void CreatePipelines(VkDevice device, VkRenderPass renderPass, MaterialInfo materialInfo) override;
    virtual VkPipeline GetBasePipeline() const override { return basePipeline; }
    virtual void Bind(VkCommandBuffer commandBuffer) const override;

private:
    VulkanPipelineManager* pipelineManager;
    VkPipeline basePipeline;
};

#endif // VULKANOPAQUEMATERIAL_H