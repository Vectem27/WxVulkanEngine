#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include <vulkan/vulkan.h>

class VulkanSurface
{
    VulkanSurface(class VulkanRenderer* renderEngine, void* windowHandle);
    ~VulkanSurface();
public:
    VkSurfaceKHR GetSurface() const { return surface; }
    uint32_t GetSupportedQueueFamilyIndex() const { return supportedQueueFamilyIndex; }
private:
    class VulkanRenderer* renderEngine;

    VkSurfaceKHR surface;
    uint32_t supportedQueueFamilyIndex;

    
private:
    void CreateSurface(void* windowHandle);

    void Cleanup();
};

#endif // VULKANSURFACE_H