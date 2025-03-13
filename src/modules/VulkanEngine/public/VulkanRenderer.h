#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.h>

class VulkanRenderer
{
public:
    VulkanRenderer(class VulkanRenderEngine* renderEngine);
    ~VulkanRenderer();

    bool RenderToSwapchain( class VulkanSwapchain* swapchain ,class IRenerable* renderObject, 
        class ICamera* camera, VkQueue graphicsQueue, VkQueue presentQueue);

private:
    class VulkanRenderEngine* renderEngine{nullptr};
};

#endif // VULKANRENDERER_H