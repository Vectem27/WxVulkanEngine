#ifndef SCENE_H
#define SCENE_H

#include "IRenderable.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "UniformBufferObject.h"
#include "Vertex.h"

class IVulkanMaterial;

class Scene : public IRenderable
{
public:
    ~Scene()
    {
        objectBuffer.Cleanup();
    }

public: // IRenderable Interface
    virtual bool Init(IRenderEngine *renderEngine) override;
    virtual void draw(const VkCommandBuffer& commandBuffer, VulkanCamera* camera) override;

    void SetMaterial(IVulkanMaterial* material);

    VkDescriptorSet* GetObjectDescriptorSet() { return &objectDescriptorSet; }

private: // Temp
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    class VulkanRenderEngine* renderEngine{ nullptr };

    std::vector<uint32_t> indices;
    std::vector<class Vertex> vertices;

    IVulkanMaterial* material{ nullptr };

    UniformBuffer objectBuffer;
    ObjectData objectData;

    VkDescriptorSet objectDescriptorSet{VK_NULL_HANDLE};
};

#endif // SCENE_H