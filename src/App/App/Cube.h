#ifndef CUBE_H
#define CUBE_H

#include "SceneComponent.h"
#include <vulkan/vulkan.h>
#include "Pipeline/IVulkanMaterial.h"
#include "UniformBufferObject.h"
#include "VulkanRenderEngine.h"
#include <vector>
#include "Vertex.h"

class Cube : public SceneComponent
{
public:
    ~Cube()
    {
        objectBuffer.Cleanup();
    }

public: // IRenderable Interface
    virtual bool Init(IRenderEngine* engine) override;
    virtual void Draw(const VkCommandBuffer& commandBuffer, ERenderPassType pass) override;

    void SetMaterial(IVulkanMaterial* material);

    VkDescriptorSet* GetObjectDescriptorSet() { return &objectDescriptorSet; }

public:
    virtual void UpdateWorldTransform() override;

private: // Temp
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<uint32_t> indices;
    std::vector<class Vertex> vertices;

    IVulkanMaterial* material{ nullptr };

    UniformBuffer objectBuffer;

    VkDescriptorSet objectDescriptorSet{VK_NULL_HANDLE};

    VulkanRenderEngine* renderEngine;
};
#endif // CUBE_H