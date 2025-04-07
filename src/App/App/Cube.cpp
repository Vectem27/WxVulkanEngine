#include "Cube.h"
#include <iostream>

/*
bool Cube::Init(IRenderEngine *engine)
{

    renderEngine = dynamic_cast<VulkanRenderEngine*>(engine);
    if (!renderEngine)
        return false;


    if (!renderEngine->GetDescriptorPoolManager()->AllocateDescriptorSets(&renderEngine->GetPipelineManager()->GetObjectDescriptorSetLayout(), 1, &objectDescriptorSet))
        throw std::runtime_error("Échec de l'allocation du descriptor set !");

    // Les vertices du carré avec des couleurs différentes pour chaque coin
    vertices = 
    {
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}},  // Vertex 1 (red)
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},  // Vertex 2 (green)
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},  // Vertex 3 (blue)
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},  // Vertex 4 (yellow)
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},  // Vertex 5 (purple)
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}},  // Vertex 6 (cyan)
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f}},  // Vertex 7 (gray)
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
    renderEngine->CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

    // Copier les données des vertices dans le buffer
    void* vertexData;
    vkMapMemory(renderEngine->GetDevice(), vertexBufferMemory, 0, vertexBufferSize, 0, &vertexData);
    memcpy(vertexData, vertices.data(), (size_t)vertexBufferSize);
    vkUnmapMemory(renderEngine->GetDevice(), vertexBufferMemory);

    // Créer le buffer d'index
    renderEngine->CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexBuffer, indexBufferMemory);

    // Copier les données des indices dans le buffer
    void* indexData;
    vkMapMemory(renderEngine->GetDevice(), indexBufferMemory, 0, indexBufferSize, 0, &indexData);
    memcpy(indexData, indices.data(), (size_t)indexBufferSize);
    vkUnmapMemory(renderEngine->GetDevice(), indexBufferMemory);

    objectBuffer.Create( renderEngine->GetDevice(), renderEngine->GetPhysicalDevice(), sizeof(ObjectData));

    UpdateWorldTransform();
    
    return true;
}

void Cube::Draw(const VkCommandBuffer &commandBuffer, ERenderPassType pass)
{
    if (material)
        material->Bind(commandBuffer, pass);


    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderEngine->GetPipelineManager()->GetPipelineLayout(), 1, 1, GetObjectDescriptorSet(), 0, nullptr);

    VkDeviceSize offset = 0;

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Cube::SetMaterial(IVulkanMaterial *material)
{
    this->material = material;
}

void Cube::UpdateWorldTransform()
{
    SceneComponent::UpdateWorldTransform();
    
    auto transformMat = GetWorldTransform().GetTransformMatrix();
    
    objectBuffer.Update(&transformMat);

    VkDescriptorBufferInfo bufferInfo {};
    bufferInfo.buffer = objectBuffer.GetBuffer(); // Supposons que ubo expose son buffer
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(ObjectData);

    VkWriteDescriptorSet descriptorWrite {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = *GetObjectDescriptorSet();
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(renderEngine->GetDevice(), 1, &descriptorWrite, 0, nullptr);
}
*/