#ifndef VULKANRENDERMESH_H
#define VULKANRENDERMESH_H

// Standard
#include <unordered_map>
#include <mutex>

// Libraries
#include <vulkan/vulkan.h>

// EngineCore
#include "Array.h"

// RHI
#include "IRenderMesh.h"

// VulkanAPI
#include "VulkanIndexBuffer.h"
#include "VulkanVertexBuffer.h"
#include "VulkanStorageBuffer.h"


namespace Vulkan 
{
    class SurfaceRenderMaterial;

    class RenderMesh : public RHI::IRenderMesh 
    {
        struct MeshInstanceData
        {
            alignas(64) Matrix4D transformMatrix;
        };

    public:
        RenderMesh();
        virtual ~RenderMesh();

    public: // IRenderable Interface
        virtual void Render(const void* renderInfo) const override; 

    public: // IRenderMesh Interface 
        virtual uint32_t AddInstance(const Transform& transform) override;
        virtual void RemoveInstance(uint32_t instanceId) override;
        virtual void SetInstanceTransform(uint32_t instanceId, const Transform& transform) override;
        virtual void UpdateRenderedInstances(bool updateAsync = false) override;
        virtual void ClearInstances() override;

        virtual void SetMaterial(uint32_t index, const RHI::IRenderMaterial* material) override;
    public:
        void SetVertices(Vertex* vertices, uint32_t vertexCount);
        void AddMeshPart(uint32_t* indices, uint32_t indexCount);

    protected:
        void RenderMeshPart(VkCommandBuffer commandBuffer, uint32_t meshPart) const;
        void UpdateInstanceDataBuffer();
    private:
    

        VulkanVertexBuffer vertexBuffer;
        Array<VulkanIndexBuffer> indexBuffers;

        Array<const SurfaceRenderMaterial*> materials;

        std::unordered_map<uint32_t, MeshInstanceData> instances;
        VulkanStorageBuffer instanceDataBuffer;

        VkDescriptorSet instanceDescriptorSet{VK_NULL_HANDLE};
        VkDescriptorPool instanceDescriptorPool{VK_NULL_HANDLE};

        std::mutex instanceMutex;
    };

} // namespace Vulkan

#endif // VULKANRENDERMESH_H