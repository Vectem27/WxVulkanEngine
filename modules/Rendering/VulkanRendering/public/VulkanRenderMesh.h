#ifndef VULKANRENDERMESH_H
#define VULKANRENDERMESH_H

// RHI
#include "IRenderMesh.h"

// VulkanAPI
#include "VulkanIndexBuffer.h"
#include "VulkanVertexBuffer.h"

namespace Vulkan 
{

    class RenderMesh : public RHI::IRenderMesh 
    {
    public:
        RenderMesh();
        virtual ~RenderMesh();

    public: // IRenderable Interface
        virtual void Render(const void* renderInfo) const override; 

    public: // IRenderMesh Interface 
        virtual uint32_t AddInstance(const Transform& transform) override;
        virtual void RemoveInstance(uint32_t instanceId) override;
        virtual void SetInstanceTransform(uint32_t instanceId, const Transform& transform) override;

    private:
        
    
    };

} // namespace Vulkan

#endif // VULKANRENDERMESH_H