#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.h>

class VulkanRenderEngine;
class VulkanShadowMapRenderTarget;
class VulkanSwapchain;
class VulkanCamera;
class VulkanLight;
class IRenderable;
class VulkanGlobalLightManager;


class VulkanRenderer
{
public:
    VulkanRenderer(VulkanRenderEngine* renderEngine);
    ~VulkanRenderer() = default;

    bool RenderToSwapchain( VulkanSwapchain* swapchain , IRenderable* renderObject, 
        VulkanCamera* camera, VulkanGlobalLightManager* lightManager, VkQueue graphicsQueue, VkQueue presentQueue);

    bool RenderToShadowMap( VulkanShadowMapRenderTarget* renderTarget ,IRenderable* renderObject, 
        VulkanCamera* light, VkQueue graphicsQueue);

protected:
    VulkanRenderEngine* renderEngine{nullptr};

    class VulkanSpotlightLightPipeline* spotlightLightPipeline;
    class VulkanPostprocessPipeline* postprocessPipeline;
};

#endif // VULKANRENDERER_H