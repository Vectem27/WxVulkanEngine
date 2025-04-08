#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.h>

class VulkanRenderEngine;
class VulkanRenderTarget;
class VulkanSwapchain;
class VulkanCamera;
class VulkanLight;
class IRenderable;
class VulkanLightManager;


class VulkanRenderer
{
public:
    VulkanRenderer(VulkanRenderEngine* renderEngine);
    ~VulkanRenderer() = default;

    bool RenderToSwapchain( VulkanSwapchain* swapchain , IRenderable* renderObject, 
        VulkanCamera* camera, VulkanLightManager* lightManager, VkQueue graphicsQueue, VkQueue presentQueue);

    bool RenderToShadowMap( VulkanRenderTarget* renderTarget ,IRenderable* renderObject, 
        VulkanCamera* light, VkQueue graphicsQueue);

protected:
    VulkanRenderEngine* renderEngine{nullptr};
};

#endif // VULKANRENDERER_H