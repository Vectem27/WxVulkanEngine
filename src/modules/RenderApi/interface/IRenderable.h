#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include "ERenderPassType.h"
#include "EngineCore.hpp"

class IRenderable
{
public:
    virtual bool ShouldRenderInPass(ERenderPassType pass) const = 0;

    virtual void CollectAllRenderChilds(Array<IRenderable*>& childs, ERenderPassType pass) = 0;
    virtual BoundingBox GetRenderBoundingBox() const = 0;

    virtual void* GetRenderMesh() const { return nullptr; }
};

#endif // IRENDERABLE_H
