#ifndef VULKANMESH_H
#define VULKANMESH_H

#include "IVulkanMesh.h" // Ensure this header is included
#include "VulkanUniformBuffer.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "EngineCore.hpp"

class IVulkanSurfaceMaterial;

class VulkanMesh : public IVulkanMesh 
{
public:
    VulkanMesh();
    virtual ~VulkanMesh();

    // TODO: Separate vertex and index buffer creation because mesh parts just changes the inex buffer offset
    void AddMeshPart(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount, IVulkanSurfaceMaterial* material);

    void SetTransform(const Transform& transform);

public: // IVulkanMesh Interface
    virtual void Draw(VkCommandBuffer commandBuffer) override;
    virtual void DrawForShadowMap(VkCommandBuffer commandBuffer) override;

    // TODO: Complete this function
    void DrawMeshPart(VkCommandBuffer commandBuffer, uint32_t partToDraw);

    // TODO: Remove this function
    void SetMaterial(IVulkanSurfaceMaterial* material) { this->material = material; }
 
private:
    uint32_t numIndices{ 0 };
    VulkanVertexBuffer vertexBuffer;
    VulkanIndexBuffer indexBuffer;

    // TODO: remove this buffer, Vulkan mesh should use flyweight pattern to save memory
    VulkanUniformBuffer meshTransformBuffer;

    IVulkanSurfaceMaterial* material{ nullptr };

    VkDescriptorSet meshDescriptorSet{ VK_NULL_HANDLE };
    VkDescriptorPool descriptorPool{ VK_NULL_HANDLE };
};

#endif // VULKANMESH_H