#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <vulkan/vulkan.h>
#include "ERenderPassType.h"
#include "EngineCore.hpp"

class IRenderable
{
public:

    // Will be removed
    virtual bool Init(class IRenderEngine* engine) = 0;
    virtual void Draw(const VkCommandBuffer& commandBuffer, ERenderPassType pass) = 0;

    // Presistent
    //ERenderPassType ShouldRenderInPass(ERenderPassType pass) const = 0;

    //virtual const Mesh* GetMesh() const = 0;
    //virtual const Material* GetMaterial() const = 0;
    //virtual BoundingBox GetLocalBoundingBox() const = 0;
};

#endif // IRENDERABLE_H
