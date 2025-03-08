#ifndef WX_VULKAN_APP_H
#define WX_VULKAN_APP_H

#include <wx/wx.h>
#include "VulkanFrame.h"

class wxVulkanApp : public wxApp 
{

public:
    virtual bool OnInit();
    //void onIdle(wxIdleEvent& evt);
    virtual int OnExit();
private:
    wxVulkanFrame* frame;
};



#endif
