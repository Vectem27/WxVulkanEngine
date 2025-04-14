#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "Mesh.h"
#include <vector>
#include "Pipeline/IVulkanMaterial.h"
#include "VulkanAPIModule.h"

#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanStagingBuffer.h"
#include "VulkanTransferManager.h"

#include "VulkanTexture.h"

#include "VulkanPipelineManager.h"

#include "VulkanMesh.h"

class CubeMesh : public IVulkanMesh
{
public:
    CubeMesh()
        : mesh(new VulkanMesh())
    {
    }

    CubeMesh(Transform transform) 
        : mesh(new VulkanMesh())
    {

    }

    virtual ~CubeMesh()
    {
        if (mesh)
            delete mesh;
    }
public:
    void SetMeshTransform(Transform newTransform)
    {
        if (mesh)
            mesh->SetTransform(newTransform);
    }

    void InitVulkanMesh()
    {
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

        indices = 
        {
            0,  1,  2,  3,  2,  1,     // Z-
            4,  5,  6,  7,  6,  5,     // Z+
            8,  9,  10, 11, 10, 9,     // X-
            12, 13, 14, 15, 14, 13,    // X+
            16, 17, 18, 19, 18, 17,    // Y+
            20, 21, 22, 23, 22, 21,    // Y-
        };

        mesh->AddMeshPart(vertices.data(), static_cast<uint32_t>(vertices.size()), indices.data(), static_cast<uint32_t>(indices.size()), nullptr);
    }
    
    virtual void DrawVulkanMesh(VkCommandBuffer commandBuffer, ERenderPassType pass) override
    {
        mesh->DrawVulkanMesh(commandBuffer, pass);
    }

    virtual void DrawVulkanMeshForShadowMap(VkCommandBuffer commandBuffer) override
    {
        mesh->DrawVulkanMeshForShadowMap(commandBuffer);
    }

    void SetMaterial(IVulkanMaterial *material)
    {
        mesh->SetMaterial(material);
    }
private:
    std::vector<uint32_t> indices;
    std::vector<Vertex> vertices;


    VulkanMesh* mesh{ nullptr };
};

#endif // CUBEMESH_H