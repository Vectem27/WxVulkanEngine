#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include "VulkanDeviceManager.h"

class VulkanSurface 
{
public:
    VulkanSurface(VkInstance instance, void* windowHandle);
    ~VulkanSurface();

    VkSurfaceKHR GetSurface() const { return surface; }

    // TODO: Mode queue management to device manager
    VkQueue GetPresentQueue() const { return presentQueue; }
    uint32_t GetPresentQueueFamilyIndex() const { return presentQueueFamilyIndex; }

private:
    void CreateSurface(VkInstance instance, void* windowHandle);
    void FindPresentQueue();

    VkInstance instance;

    VkSurfaceKHR surface{ VK_NULL_HANDLE };
    VkQueue presentQueue{ VK_NULL_HANDLE };
    uint32_t presentQueueFamilyIndex{ UINT32_MAX };
};

#endif // VULKANSURFACE_H