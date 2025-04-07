#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.h>

class VulkanRenderEngine;
class VulkanRenderTarget;
class VulkanSwapchain;
class VulkanCamera;
class VulkanLight;
class IRenderable;


class VulkanRenderer
{
public:
    VulkanRenderer(VulkanRenderEngine* renderEngine);
    ~VulkanRenderer() = default;

    bool RenderToSwapchain( VulkanSwapchain* swapchain , IRenderable* renderObject, 
        VulkanCamera* camera, VkQueue graphicsQueue, VkQueue presentQueue);

    bool RenderToShadowMap( VulkanRenderTarget* renderTarget ,IRenderable* renderObject, 
        VulkanLight* light, VkQueue graphicsQueue);

protected:
    VulkanRenderEngine* renderEngine{nullptr};
};

#endif // VULKANRENDERER_H