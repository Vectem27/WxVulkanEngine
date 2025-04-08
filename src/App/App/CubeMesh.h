#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "Mesh.h"
#include "Vertex.h"
#include <vector>
#include "Pipeline/IVulkanMaterial.h"
#include "VulkanRenderEngine.h"

class CubeMesh : public Mesh
{
public:
    CubeMesh() = default;
    CubeMesh(Transform transform) : Mesh(transform) {}

public:
    virtual void InitVulkanMesh(VulkanRenderEngine* vulkanRenderEngine) override
    {
        Mesh::InitVulkanMesh(vulkanRenderEngine);
        
        // Les vertices du cube avec des couleurs différentes pour chaque coin
        vertices = 
        {
            // Z+
            {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},  // Vertex 1 (red)
            {{-0.5f,  0.5f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},  // Vertex 3 (blue)
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},  // Vertex 2 (green)
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},  // Vertex 4 (yellow)

            // Z-
            {{-0.5f, -0.5f,  0.5f}, {0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // Vertex 5 (purple)
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // Vertex 6 (cyan)
            {{-0.5f,  0.5f,  0.5f}, {0.5f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // Vertex 7 (gray)
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},  // Vertex 8 (white)

            // X-
            {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 9
            {{-0.5f, -0.5f,  0.5f}, {0.5f, 0.5f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // Vertex 10
            {{-0.5f,  0.5f, -0.5f}, {0.5f, 1.0f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},  // Vertex 11
            {{-0.5f,  0.5f,  0.5f}, {0.5f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Vertex 12

            // X+
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 13
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},  // Vertex 15
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // Vertex 14
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Vertex 16

            // Y+
            {{-0.5f,  0.5f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 17
            {{-0.5f,  0.5f,  0.5f}, {0.5f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},  // Vertex 19
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // Vertex 18
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // Vertex 20

            // Y-
            {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 21
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},  // Vertex 22
            {{-0.5f, -0.5f,  0.5f}, {0.5f, 0.5f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},  // Vertex 23
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},  // Vertex 24
        };

        // Indices pour former les faces du cube avec deux triangles par face
        indices = 
        {
            0,  1,  2,  3,  2,  1,     // Z-
            4,  5,  6,  7,  6,  5,     // Z+
            8,  9,  10, 11, 10, 9,     // X-
            12, 13, 14, 15, 14, 13,    // X+
            16, 17, 18, 19, 18, 17,    // Y+
            20, 21, 22, 23, 22, 21,    // Y-
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

    }
    
    virtual void DrawVulkanMesh(VkCommandBuffer commandBuffer, ERenderPassType pass) override
    {
        if (material)
            material->Bind(commandBuffer, pass);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetVulkanRenderEngine()->GetPipelineManager()->GetPipelineLayout(), 1, 1, &GetVulkanMeshDescriptorSet(), 0, nullptr);

        VkDeviceSize offset = 0;

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    }

    void SetMaterial(IVulkanMaterial *material)
    {
        this->material = material;
    }
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