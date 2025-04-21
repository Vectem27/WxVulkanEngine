#ifndef VULKANRENDERTARGETRENDERER_H
#define VULKANRENDERTARGETRENDERER_H

#include <vulkan/vulkan.h>
#include <functional>

class IVulkanRenderTarget;
class VulkanCamera;
class IVulkanMesh;

class IVulkanLightManager;

class VulkanRenderTargetRenderer
{
private:
    void Init(VkQueue graphicsQueue, uint32_t queueFamilyIndex);
    VkQueue GetGraphicsQueue() const { return graphicsQueue; }

    void SetGeometryRenderPass(VkRenderPass pass) { geometryPass = pass; }
    VkRenderPass GetGeometryRenderPass() const { return geometryPass; }

    void SetLightingRenderPass(VkRenderPass pass) { lightingPass = pass; }
    VkRenderPass GetLightingRenderPass() const { return lightingPass; }

    void SetPostprocessRenderPass(VkRenderPass pass) { postprocessPass = pass; }
    VkRenderPass GetPostprocessRenderPass() const { return postprocessPass; }

    void Shutdown() noexcept;

    friend class VulkanAPIModule;
public:
    static VulkanRenderTargetRenderer& GetInstance()
    {
        static VulkanRenderTargetRenderer instance;
        return instance;
    }

    void Render(IVulkanRenderTarget* renderTarget, VulkanCamera *lightViewCamera, IVulkanMesh **meshes, uint32_t meshNumber, const IVulkanLightManager& lightManager, std::function<void(VkCommandBuffer)> renderCallback = nullptr) const;

private:
    VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
    VkCommandPool commandPool{VK_NULL_HANDLE};
    VkRenderPass geometryPass{VK_NULL_HANDLE};
    VkRenderPass lightingPass{VK_NULL_HANDLE};
    VkRenderPass postprocessPass{VK_NULL_HANDLE};
    VkQueue graphicsQueue{VK_NULL_HANDLE};

    // TODO: Remove
    class VulkanPostprocessPipeline* postprocessPipeline;
};

VulkanRenderTargetRenderer& GetVulkanRenderTargetRenderer();

#endif // VULKANRENDERTARGETRENDERER_H
