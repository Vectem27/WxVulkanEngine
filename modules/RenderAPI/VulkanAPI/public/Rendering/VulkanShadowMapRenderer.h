#ifndef VULKANSHADOWMAPRENDERER_H
#define VULKANSHADOWMAPRENDERER_H

#include <vulkan/vulkan.h>
#include <functional>

class IVulkanMesh;
class VulkanShadowMapCamera;

class VulkanShadowMapRenderer
{
private:
    void Init(VkQueue graphicsQueue, uint32_t queueFamilyIndex);
    void SetRenderPass(VkRenderPass pass) { renderPass = pass; }
    VkQueue GetGraphicsQueue() const { return graphicsQueue; }
    VkRenderPass GetRenderPass() const { return renderPass; }
    void Shutdown() noexcept;

    friend class VulkanAPIModule;
public:
    static VulkanShadowMapRenderer& GetInstance()
    {
        static VulkanShadowMapRenderer instance;
        return instance;
    }

    void Render(VulkanShadowMapCamera* shadowMapCamera, IVulkanMesh** meshes, uint32_t meshNumber, std::function<void(VkCommandBuffer)> renderCallback = nullptr);

private:
    VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
    VkCommandPool commandPool{VK_NULL_HANDLE};
    VkRenderPass renderPass{VK_NULL_HANDLE};
    VkQueue graphicsQueue{VK_NULL_HANDLE};
};

VulkanShadowMapRenderer& GetVulkanShadowMapRenderer();

#endif // VULKANSHADOWMAPRENDERER_H
