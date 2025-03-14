#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <vulkan/vulkan.h>


class IRenderable
{
public:
    virtual bool Init(class IRenderEngine* engine) = 0;
    virtual void draw(const VkCommandBuffer& commandBuffer) = 0;
};

#endif // IRENDERABLE_H
