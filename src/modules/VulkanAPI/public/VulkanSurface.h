#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include <vulkan/vulkan.h>

class VulkanSurface 
{
public:
    VulkanSurface(void* windowHandle);
    ~VulkanSurface();

    VkSurfaceKHR GetSurface() const { return surface; }

    VkQueue GetPresentQueue() const { return presentQueue; }
    uint32_t GetPresentQueueFamilyIndex() const { return presentQueueFamilyIndex; }

private:
    void CreateSurface(void* windowHandle);
    void FindPresentQueue();

    VkSurfaceKHR surface{ VK_NULL_HANDLE };
    VkQueue presentQueue{ VK_NULL_HANDLE };
    uint32_t presentQueueFamilyIndex{ UINT32_MAX };
};

#endif // VULKANSURFACE_H