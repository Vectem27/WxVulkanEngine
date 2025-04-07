#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "Mesh.h"
#include "Vertex.h"
#include <vector>
#include "Pipeline/IVulkanMaterial.h"
#include "VulkanRenderEngine.h"
#include "VulkanLight.h"

class CubeMesh : public Mesh
{
public:
    CubeMesh() = default;
    CubeMesh(Transform transform) : Mesh(transform) {}

public:
    virtual void InitVulkanMesh(VulkanRenderEngine* vulkanRenderEngine) override
    {
        Mesh::InitVulkanMesh(vulkanRenderEngine);
        
        // Les vertices du carré avec des couleurs différentes pour chaque coin
        vertices = 
        {
            {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}},  // Vertex 1 (red)
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.5f}},  // Vertex 2 (green)
            {{-0.5f,  0.5f, -0.5f}, {0.5f, 1.0f, 0.5f}},  // Vertex 3 (blue)
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.5f}},  // Vertex 4 (yellow)
            {{-0.5f, -0.5f,  0.5f}, {0.5f, 0.5f, 1.0f}},  // Vertex 5 (purple)
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 1.0f}},  // Vertex 6 (cyan)
            {{-0.5f,  0.5f,  0.5f}, {0.5f, 1.0f, 1.0f}},  // Vertex 7 (gray)
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}},  // Vertex 8 (white)
        };

        // Indices pour former les faces du cube avec deux triangles par face
        indices = 
        {
            0, 2, 1,   1, 2, 3,
            5, 7, 4,   4, 7, 6,
            4, 6, 0,   0, 6, 2,
            1, 3, 5,   5, 3, 7,
            2, 6, 3,   3, 6, 7,
            4, 0, 5,   5, 0, 1,
        };

        // Définir la taille des buffers
        VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
        VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

        // Créer le buffer de vertex
        GetVulkanRenderEngine()->CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

        // Copier les données des vertices dans le buffer
        void* vertexData;
        vkMapMemory(GetVulkanRenderEngine()->GetDevice(), vertexBufferMemory, 0, vertexBufferSize, 0, &vertexData);
        memcpy(vertexData, vertices.data(), (size_t)vertexBufferSize);
        vkUnmapMemory(GetVulkanRenderEngine()->GetDevice(), vertexBufferMemory);

        // Créer le buffer d'index
        GetVulkanRenderEngine()->CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexBuffer, indexBufferMemory);

        // Copier les données des indices dans le buffer
        void* indexData;
        vkMapMemory(GetVulkanRenderEngine()->GetDevice(), indexBufferMemory, 0, indexBufferSize, 0, &indexData);
        memcpy(indexData, indices.data(), (size_t)indexBufferSize);
        vkUnmapMemory(GetVulkanRenderEngine()->GetDevice(), indexBufferMemory);

        light.InitVulkanLight(vulkanRenderEngine);
    }
    
    virtual void DrawVulkanMesh(VkCommandBuffer commandBuffer, ERenderPassType pass) const override
    {
        if (material)
            material->Bind(commandBuffer, pass);


        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetVulkanRenderEngine()->GetPipelineManager()->GetPipelineLayout(), 1, 1, &GetVulkanMeshDescriptorSet(), 0, nullptr);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetVulkanRenderEngine()->GetPipelineManager()->GetPipelineLayout(), 2, 1, &light.GetLightDescriptorSet(), 0, nullptr);

        VkDeviceSize offset = 0;

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    }

    void SetMaterial(IVulkanMaterial *material)
    {
        this->material = material;
    }
public:
    VulkanLight light;
private:

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<uint32_t> indices;
    std::vector<class Vertex> vertices;

    IVulkanMaterial* material{ nullptr };
};

#endif // CUBEMESH_H