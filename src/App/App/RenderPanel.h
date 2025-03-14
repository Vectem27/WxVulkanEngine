#ifndef RENDERPANEL_H
#define RENDERPANEL_H

#include <wx/wx.h>
#include <vulkan/vulkan.h>
#include "Scene.h"
#include "VulkanMaterial.h"
#include "VulkanRenderer.h"
#include "VulkanSwapchain.h"
#include "VulkanCamera.h"

class RenderPanel : public wxWindow
{
public:
    RenderPanel(wxWindow* parent) 
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
    {
        SetWindowLongPtr(GetHWND(), GWL_STYLE, GetWindowLongPtr(GetHWND(), GWL_STYLE) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        renderEngine = new VulkanRenderEngine();

        try
        {
            renderEngine->Init(reinterpret_cast<void *>(GetHandle()));
            
            surface = new VulkanSurface(renderEngine->GetInstance(), renderEngine->GetDeviceManager(), reinterpret_cast<void *>(GetHandle()));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }


        swapchain = new VulkanSwapchain(renderEngine, surface);
        swapchain->Create(renderEngine->GetDefaultRenderPass());

        renderer = new VulkanRenderer(renderEngine);

        camera = new VulkanCamera();
        
        camera->Init(renderEngine, swapchain);
  
        scene = new Scene();
        scene->Init(renderEngine);
  
        MaterialInfo matInfo = {};
        matInfo.fragmentShader = "shaders/shader.frag";
        matInfo.vertexShader = "shaders/shader.vert";

        material.Init(renderEngine, matInfo);
    }

    void Cleanup()
    {
        camera->Cleanup();
        delete camera;
        delete scene;
        delete swapchain;
        delete renderer;

        if (renderEngine)
        {
            renderEngine->Shutdown();
            delete renderer;
        }
    }


    void Render()
    {
        try
        {
            //camera->Render(scene);
            renderer->RenderToSwapchain(swapchain, scene, camera, renderEngine->GetDeviceManager()->GetGraphicsQueue(), surface->GetPresentQueue());
        }
        catch (const std::exception &e)
        {
            wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
        }
    }

public:
    VulkanRenderEngine *renderEngine;
    VulkanCamera* camera;
    VulkanMaterial material;
    Scene* scene;

    VulkanRenderer* renderer;
    VulkanSwapchain* swapchain;
    VulkanSurface* surface;
};


#endif // RENDERPANEL_H