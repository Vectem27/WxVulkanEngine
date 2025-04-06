#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <vulkan/vulkan.h>


class IRenderable
{
public:
    virtual bool Init(class IRenderEngine* engine) = 0;
    virtual void Draw(const VkCommandBuffer& commandBuffer, class VulkanCamera* camera) = 0;
};

#endif // IRENDERABLE_H
