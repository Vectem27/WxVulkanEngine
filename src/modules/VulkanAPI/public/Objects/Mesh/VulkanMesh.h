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

    void AddMeshPart(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount, IVulkanSurfaceMaterial* material);

    void SetTransform(const Transform& transform);

public: // IVulkanMesh Interface
    virtual void Draw(VkCommandBuffer commandBuffer) override;
    virtual void DrawForShadowMap(VkCommandBuffer commandBuffer) override;

    // TODO: Remove this function
    void SetMaterial(IVulkanSurfaceMaterial* material) { this->material = material; }
 
private:
    uint32_t numIndices{ 0 };
    VulkanVertexBuffer vertexBuffer;
    VulkanIndexBuffer indexBuffer;
    VulkanUniformBuffer meshTransformBuffer;

    IVulkanSurfaceMaterial* material{ nullptr };

    VkDescriptorSet meshDescriptorSet{ VK_NULL_HANDLE };
    VkDescriptorPool descriptorPool{ VK_NULL_HANDLE };
};

#endif // VULKANMESH_H