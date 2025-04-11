#ifndef RENDERSURFACE_H
#define RENDERSURFACE_H


#include <wx/wx.h>
#include <vulkan/vulkan.h>
#include "VulkanSurface.h"

class RenderSurface : public wxWindow
{
public:
    RenderSurface(wxWindow* parent) 
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
    {
        SetWindowLongPtr(GetHWND(), GWL_STYLE, GetWindowLongPtr(GetHWND(), GWL_STYLE) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    }

public:
    bool IsVulkanInitialized() const { return isVulkanInitialized; }

    void InitVulkanSurface(VulkanRenderEngine* vulkanRenderEngine)
    {
        if (!vulkanRenderEngine)
            throw std::invalid_argument("Render engine reference is null");

        this->vulkanRenderEngine = vulkanRenderEngine;

        surface = new VulkanSurface(vulkanRenderEngine->GetInstance(), vulkanRenderEngine->GetDeviceManager(), reinterpret_cast<void *>(GetHandle()));

        isVulkanInitialized = true;
    }

    VulkanSurface* GetVulkanSurface() const { return surface; }
private:
    bool isVulkanInitialized{false};
    VulkanRenderEngine* vulkanRenderEngine{nullptr};

    VulkanSurface* surface;
};

#endif // RENDERSURFACE_H
