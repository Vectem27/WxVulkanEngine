#ifndef RENDERPANEL_H
#define RENDERPANEL_H

#include <wx/wx.h>
#include <vulkan/vulkan.h>
#include "Scene.h"
#include "VulkanMaterial.h"


class RenderPanel : public wxWindow
{
public:
    RenderPanel(wxWindow* parent) 
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
    {
        SetWindowLongPtr(GetHWND(), GWL_STYLE, GetWindowLongPtr(GetHWND(), GWL_STYLE) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        renderer = new VulkanRenderEngine();

        if (!rendererInitialized)
        {
#ifdef __WXMSW__
            // Pour Windows, récupération du handle natif.
            renderer->Init(reinterpret_cast<void *>(GetHWND()));
#elif defined(__WXGTK__)  // Pour Linux (GTK)
            // Pour Linux, récupération du handle natif pour un panel GTK.
            renderer->Init(reinterpret_cast<void *>(GetHandle()));
#elif defined(__WXMAC__)  // Pour macOS
            // Pour macOS, récupération du handle natif.
            renderer->Init(reinterpret_cast<void *>(GetHandle()));
#else
            // Pour d'autres plateformes, il faudra récupérer le handle natif approprié.
            renderer->Init(nullptr);
    #endif
            rendererInitialized = true;
        }

        MaterialInfo matInfo = {};
        matInfo.fragmentShader = "shaders/shader.frag";
        matInfo.vertexShader = "shaders/shader.vert";

        material.Init(renderer, matInfo);

        camera = new VulkanCamera();
        swapchainRenderer = new SwapchainRenderer(renderer->surface, renderer->graphicsQueueFamilyIndex);
        swapchainRenderer->Init(renderer);
        camera->Init(renderer, swapchainRenderer);
  
        scene = new Scene();
        scene->Init(renderer);
        
    }

    void Cleanup()
    {
        swapchainRenderer->Cleanup();
        delete swapchainRenderer;
        camera->Cleanup();
        delete camera;
        delete scene;

        if (renderer)
        {
            renderer->Shutdown();
            delete renderer;
        }
    }


    void Render()
    {
        try
        {
            camera->Render(scene);
        }
        catch (const std::exception &e)
        {
            wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
        }
    }

public:
    VulkanRenderEngine *renderer;
    VulkanCamera* camera;
    SwapchainRenderer* swapchainRenderer;
    VulkanMaterial material;
    Scene* scene;
private:
    bool rendererInitialized{false};
};


#endif // RENDERPANEL_H