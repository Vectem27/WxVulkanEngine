#ifndef MATERIAL_H
#define MATERIAL_H

#include "IVulkanMaterial.h"

class Material : public IVulkanMaterial
{
public:
    virtual void InitVulkanMaterial(class VulkanRenderEngine* vulkanRenderEngine) override;
    virtual void BindVulkanMaterial(VkCommandBuffer commandBuffer, ERenderPassType pass) const override;
};

#endif // MATERIAL_H