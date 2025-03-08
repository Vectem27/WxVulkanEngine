#include "App.h"

bool wxVulkanApp::OnInit() 
{
    frame = new wxVulkanFrame("Vulkan avec wxWidgets");
    frame->Show(true);
    return true;
}

int wxVulkanApp::OnExit()
{
    frame->Cleanup();
    return wxApp::OnExit();
}
