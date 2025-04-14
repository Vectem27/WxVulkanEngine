#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "VulkanRenderTarget.h"
#include "Engine.h"
#include "IRenderTarget.h"
#include "Logger.h"

class RenderTarget : IRenderTarget
{
public:
    //RenderTarget(size_t width, size_t height, size_t depth, size_t layers, size_t mipLevels, size_t samples)
    //    : vulkanRenderTarget(new VulkanRenderTarget(width, height, depth, layers, mipLevels, samples))
    //{
    //};
    RenderTarget(size_t width, size_t height)
    {
        switch (GetEngine().GetRenderAPI())
        {
        case RenderAPI::Vulkan:
            vulkanRenderTarget = new VulkanRenderTarget();
            vulkanRenderTarget->Init(width, height);
            break;
        default:
            Log(Error, "LogRendering", "RenderAPI not supported");
            break;
        }
    }
    
    virtual ~RenderTarget()
    {
        if (vulkanRenderTarget)
        {
            delete vulkanRenderTarget;
            vulkanRenderTarget = nullptr;
        }
    }

    virtual void* GetRenderTarget(RenderAPI renderAPI) override { return static_cast<void*>(vulkanRenderTarget); }

private:
    VulkanRenderTarget* vulkanRenderTarget{nullptr};

};
#endif // RENDERTARGET_H