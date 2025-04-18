#ifndef IVULKANMESH_H
#define IVULKANMESH_H

#include <vulkan/vulkan.h>

class IVulkanMesh
{
public:
    virtual void Draw(VkCommandBuffer commandBuffer) = 0;
    virtual void DrawForShadowMap(VkCommandBuffer commandBuffer) = 0;
};

#endif // IVULKANMESH_H