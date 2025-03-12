#include "VulkanSurface.h"
#include "VulkanRenderer.h"

VulkanSurface::VulkanSurface(VulkanRenderer *renderEngine, void* windowHandle)
    : renderEngine(renderEngine)
{
    CreateSurface(windowHandle);
}

VulkanSurface::~VulkanSurface()
{

}

void VulkanSurface::CreateSurface(void* windowHandle)
{
#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR createInfo{}; 
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = reinterpret_cast<HWND>(windowHandle);
    createInfo.hinstance = GetModuleHandle(nullptr);
    if (vkCreateWin32SurfaceKHR(renderEngine->GetInstance(), &createInfo, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
#elif defined(__linux__)  // Pour Linux
    VkXcbSurfaceCreateInfoKHR createInfo{}; 
    createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfo.connection = XGetXCBConnection(XOpenDisplay(nullptr));
    createInfo.window = reinterpret_cast<xcb_window_t>(windowHandle);
    if (vkCreateXcbSurfaceKHR(renderEngine->GetInstance(), &createInfo, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create XCB window surface!");
    }
#elif defined(__APPLE__)  // Pour macOS
    VkCocoaSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COCOA_SURFACE_CREATE_INFO_KHR;
    createInfo.pView = windowHandle;  // Utilisation du handle natif macOS
    if (vkCreateCocoaSurfaceKHR(renderEngine->GetInstance(), &createInfo, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create Cocoa window surface!");
    }
#else
    // Autres plateformes
    throw std::runtime_error("Surface creation not supported for this platform!");
#endif
}

void VulkanSurface::Cleanup()
{

}
