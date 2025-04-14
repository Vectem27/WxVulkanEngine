#ifndef SWAPCHAINTARGET_H
#define SWAPCHAINTARGET_H

#include "VulkanSwapchain.h"
#include "Engine.h"
#include "Logger.h"
#include "IRenderTarget.h"
#include "VulkanSurface.h"

class SwapchainTarget : public IRenderTarget {
public:
    SwapchainTarget(VulkanSurface* surface)
    {
        switch (GetEngine().GetRenderAPI())
        {
        case RenderAPI::Vulkan:
            vulkanSwapchain = new VulkanSwapchain(surface);
            break;
        default:
            Log(Error, "LogRendering", "RenderAPI not supported");
            break;
        }
    }
    virtual ~SwapchainTarget()
    {
        if (vulkanSwapchain)
        {
            delete vulkanSwapchain;
            vulkanSwapchain = nullptr;
        }
    }

    virtual void* GetRenderTarget(RenderAPI renderAPI) override
    {
        switch (GetEngine().GetRenderAPI())
        {
        case RenderAPI::Vulkan:
            return static_cast<void*>(vulkanSwapchain);
        default:
            Log(Error, "LogRendering", "RenderAPI not supported");
            return nullptr;
        }
    }   

private:
    VulkanSwapchain* vulkanSwapchain;
};

#endif // SWAPCHAINTARGET_H
