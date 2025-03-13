#include "Scene.h"
#include "VulkanRenderEngine.h"
#include "IRenderTarget.h"


bool Scene::Init(IRenderEngine *renderEngine)
{

    this->renderEngine = dynamic_cast<VulkanRenderEngine*>(renderEngine);
    if (!this->renderEngine)
    {
        return false;
    }


    // Les vertices du carré avec des couleurs différentes pour chaque coin
    vertices = 
    {
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},  // Vertex 1 (red)
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},  // Vertex 2 (green)
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},  // Vertex 3 (blue)
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},  // Vertex 4 (yellow)
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}},  // Vertex 5 (purple)
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}},  // Vertex 6 (cyan)
        {{-0.5f,  0.5f,  0.5f}, {0.1f, 0.1f, 0.1f}},  // Vertex 7 (gray)
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
    this->renderEngine->CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

    // Copier les données des vertices dans le buffer
    void* vertexData;
    vkMapMemory(this->renderEngine->GetDevice(), vertexBufferMemory, 0, vertexBufferSize, 0, &vertexData);
    memcpy(vertexData, vertices.data(), (size_t)vertexBufferSize);
    vkUnmapMemory(this->renderEngine->GetDevice(), vertexBufferMemory);

    // Créer le buffer d'index
    this->renderEngine->CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexBuffer, indexBufferMemory);

    // Copier les données des indices dans le buffer
    void* indexData;
    vkMapMemory(this->renderEngine->GetDevice(), indexBufferMemory, 0, indexBufferSize, 0, &indexData);
    memcpy(indexData, indices.data(), (size_t)indexBufferSize);
    vkUnmapMemory(this->renderEngine->GetDevice(), indexBufferMemory);


    return true;
}

void Scene::draw(ICamera *camera)
{
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(camera->GetRenderTarget()->GetCurrentCommandBuffer(), 0, 1, &vertexBuffer, &offset);
    vkCmdBindIndexBuffer(camera->GetRenderTarget()->GetCurrentCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(camera->GetRenderTarget()->GetCurrentCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    //vkCmdDraw(camera->GetRenderTarget()->GetCurrentCommandBuffer(), 6, 1, 0, 0);
}
