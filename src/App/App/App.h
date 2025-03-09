#ifndef WX_VULKAN_APP_H
#define WX_VULKAN_APP_H

#include <wx/wx.h>
#include "VulkanFrame.h"
#include <chrono>

class wxVulkanApp : public wxApp
{

public:
    virtual bool OnInit();
    void onIdle(wxIdleEvent &evt);
    virtual int OnExit();

private:
    wxVulkanFrame *frame;
    std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
    double fps = 0.0;
};

#endif
