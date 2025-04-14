#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "ICameraComponent.h"
#include "VulkanCamera.h"
#include "Engine.h"

#include "RenderTarget.h"
#include "Logger.h"


class CameraComponent : public ICameraComponent {
public:
    CameraComponent()
    {
        switch (GetEngine().GetRenderAPI()) 
        {
        case RenderAPI::Vulkan:
            vulkanCamera = new VulkanCamera();:
            break;
        default:
            Log(Error, "LogRendering", "RenderAPI not supported");
            break;
        }
    }

    virtual ~CameraComponent()
    {
        if (vulkanCamera) 
        {
            delete vulkanCamera;
            vulkanCamera = nullptr;
        }
    }
    

public:
    void* GetRenderCamera(RenderAPI renderAPI) override {
        switch (GetEngine().GetRenderAPI())     
        {
        case RenderAPI::Vulkan:
            return static_cast<void*>(vulkanCamera); 
        
        default:
            Log(Error, "LogRendering", "RenderAPI not supported");
            return nullptr;
        }
    }

    void SetRenderTarget(RenderTarget* renderTarget) 
    {
        if (!renderTarget) 
        {
            Log(Error, "LogRendering", "Render target is null");
            return;
        }
        this->renderTarget = renderTarget;

        switch (GetEngine().GetRenderAPI()) 
        {
        case RenderAPI::Vulkan:
            if (vulkanCamera) 
            {
                vulkanCamera->SetRenderTarget(static_cast<VulkanRenderTarget*>(renderTarget->GetVulkanRenderTarget()));
            }
            break;
        default:
            Log(Error, "LogRendering", "RenderAPI not supported");
            break;
        }
    }

private:
    VulkanCamera* vulkanCamera{nullptr};
    RenderTarget* renderTarget{nullptr};
};

#endif // CAMERACOMPONENT_H