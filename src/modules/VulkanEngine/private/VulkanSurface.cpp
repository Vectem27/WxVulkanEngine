#include "VulkanSurface.h"

#include <stdexcept>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
    #include <vulkan/vulkan_win32.h>
#elif defined(__linux__)
    #include <vulkan/vulkan_xcb.h> // Remplace par xlib ou wayland si nécessaire
#endif

VulkanSurface::VulkanSurface(VkInstance instance, const VulkanDeviceManager *deviceManager, void *windowHandle)
    : instance(instance)
{
    CreateSurface(instance, windowHandle);
    FindPresentQueue(deviceManager);
}

VulkanSurface::~VulkanSurface()
{
    if (surface != VK_NULL_HANDLE) 
    {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }
}

void VulkanSurface::CreateSurface(VkInstance instance, void *windowHandle)
{
#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = static_cast<HWND>(windowHandle);
    createInfo.hinstance = GetModuleHandle(nullptr);

    if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create Win32 Vulkan surface!");
    }
#elif defined(__linux__)
    // Exemple pour XCB (remplace avec Xlib ou Wayland si nécessaire)
    VkXcbSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfo.connection = static_cast<xcb_connection_t*>(windowHandle);
    createInfo.window = static_cast<xcb_window_t>(windowHandle);

    if (vkCreateXcbSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create XCB Vulkan surface!");
    }
#endif
}

void VulkanSurface::FindPresentQueue(const VulkanDeviceManager *deviceManager)
{
    VkPhysicalDevice physicalDevice = deviceManager->GetPhysicalDevice();
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilyCount; i++) 
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if (presentSupport) 
        {
            presentQueueFamilyIndex = i;
            vkGetDeviceQueue(deviceManager->GetDevice(), i, 0, &presentQueue);
            return;
        }
    }

    throw std::runtime_error("Failed to find a suitable present queue!");
}
