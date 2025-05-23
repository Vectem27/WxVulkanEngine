#ifndef IRENDERMESH_H
#define IRENDERMESH_H

#include <inttypes.h>

//TODO: Change to Transform.h when it will be created
#include "EngineCore.hpp"

#include "IRenderable.h"

namespace RHI
{
    class IRenderMaterial;
    
    class IRenderMesh : public IRenderable
    {
    public:
        virtual ~IRenderMesh() = default;

        virtual uint32_t AddInstance(const Transform& transform) = 0;
        virtual void RemoveInstance(uint32_t instanceId) = 0;
        virtual void SetInstanceTransform(uint32_t instanceId, const Transform& transform) = 0;
        virtual void UpdateRenderedInstances(bool updateAsync = false) = 0;
        virtual void ClearInstances() = 0;

        virtual void SetMaterial(uint32_t index, const IRenderMaterial* material) = 0;
    };
}

#endif // IRENDERMESH_H