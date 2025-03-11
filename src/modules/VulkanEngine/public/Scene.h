#ifndef SCENE_H
#define SCENE_H

#include "IRenderable.h"
#include <vulkan/vulkan.h>
#include <vector>


class Scene : public IRenderable
{
public: // IRenderable Interface
    virtual bool Init(class IRenderEngine* engine) override;
    virtual void draw(class ICamera* camera) override;

private: // Temp
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    class VulkanRenderer* renderEngine;

    std::vector<uint32_t> indices;
    std::vector<class Vertex> vertices;
};

#endif // SCENE_H