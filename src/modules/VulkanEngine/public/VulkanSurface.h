#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include "VulkanDeviceManager.h"

class VulkanSurface 
{
public:
    VulkanSurface(VkInstance instance, const VulkanDeviceManager* deviceManager, void* windowHandle);
    ~VulkanSurface();

    VkSurfaceKHR GetSurface() const { return surface; }
    VkQueue GetPresentQueue() const { return presentQueue; }
    uint32_t GetPresentQueueFamilyIndex() const { return presentQueueFamilyIndex; }

private:
    void CreateSurface(VkInstance instance, void* windowHandle);
    void FindPresentQueue(const VulkanDeviceManager* deviceManager);

    VkInstance instance;

    VkSurfaceKHR surface{ VK_NULL_HANDLE };
    VkQueue presentQueue{ VK_NULL_HANDLE };
    uint32_t presentQueueFamilyIndex{ UINT32_MAX };
};

#endif // VULKANSURFACE_H