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
    void InitVulkanSurface()
    {
        surface = new VulkanSurface(reinterpret_cast<void *>(GetHandle()));
    }

    VulkanSurface* GetVulkanSurface() { return surface; }
private:

    VulkanSurface* surface{nullptr};
};

#endif // RENDERSURFACE_H
