#include "App.h"

bool wxVulkanApp::OnInit()
{
    frame = new wxVulkanFrame("Vulkan avec wxWidgets");
    frame->Show(true);
    Bind(wxEVT_IDLE, &wxVulkanApp::onIdle, this);
    return true;
}

void wxVulkanApp::onIdle(wxIdleEvent &evt)
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - lastFrameTime;
    lastFrameTime = now;

    fps = 1.0 / elapsed.count(); // Calcul des FPS
    frame->SetTitle(wxString::Format("Vulkan avec wxWidgets - FPS: %.0f", fps));

    frame->Render();
    evt.RequestMore();
}

int wxVulkanApp::OnExit()
{
    frame->Cleanup();
    return wxApp::OnExit();
}
