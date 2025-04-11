#ifndef IVULKANMATERIAL_H
#define IVULKANMATERIAL_H

#include <vulkan/vulkan.h>
#include "ERenderPassType.h"

class IVulkanMaterial
{
public:
    virtual void InitVulkanMaterial(class VulkanRenderEngine* vulkanRenderEngine) = 0;
    virtual void BindVulkanMaterial(VkCommandBuffer commandBuffer, ERenderPassType pass) const = 0;
};
 
#endif // IVULKANMATERIAL_H