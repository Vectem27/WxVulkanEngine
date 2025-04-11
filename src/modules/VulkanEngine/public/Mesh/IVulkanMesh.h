#ifndef IVULKANMESH_H
#define IVULKANMESH_H

#include <vulkan/vulkan.h>
#include "ERenderPassType.h"

class IVulkanMesh
{
public:
    virtual void InitVulkanMesh(class VulkanRenderEngine* vulkanRenderEngine) = 0;
    virtual void DrawVulkanMesh(VkCommandBuffer commandBuffer, ERenderPassType pass) = 0;
    virtual void DrawVulkanMeshForShadowMap(VkCommandBuffer commandBuffer) = 0;
};

#endif // IVULKANMESH_H