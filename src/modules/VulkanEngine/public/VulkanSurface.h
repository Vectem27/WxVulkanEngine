#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include <vulkan/vulkan.h>

class VulkanSurface
{
    VulkanSurface(class VulkanRenderEngine* renderEngine, void* windowHandle);
    ~VulkanSurface();
public:
    VkSurfaceKHR GetSurface() const { return surface; }
    uint32_t GetSupportedQueueFamilyIndex() const { return supportedQueueFamilyIndex; }
private:
    class VulkanRenderEngine* renderEngine;

    VkSurfaceKHR surface;
    uint32_t supportedQueueFamilyIndex;

    
private:
    void CreateSurface(void* windowHandle);

    void Cleanup();
};

#endif // VULKANSURFACE_H