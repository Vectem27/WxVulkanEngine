#ifndef WX_VULKAN_APP_H
#define WX_VULKAN_APP_H

#include <wx/wx.h>
#include "VulkanFrame.h"
#include <chrono>
#include "RenderSurface.h"
#include <vulkan/vulkan.h>

class wxVulkanApp : public wxApp
{
public: 
    wxVulkanApp();
public:
    virtual bool OnInit();
    void onIdle(wxIdleEvent &evt);
    virtual int OnExit();

private:
    void InitVulkan();
    void RenderVulkan();
    void ShutdownVulkan();

private:
    wxVulkanFrame *frame;
    std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
    double fps = 0.0;

private:
    class VulkanRenderEngine *vulkanRenderEngine;
    class CameraComponent* camera;
    class Actor* cubeActor;
    class Cube* cube;
    class Cube* tinyCube;
    class Cube* floor;
    class VulkanOpaqueMaterial* material;

    class VulkanRenderer* renderer;
    class VulkanSwapchain* swapchain;
    class World* world;
    class VulkanGlobalLightManager* lightManager;
    class ProjectorLightComponent* projLight;
};

#endif
