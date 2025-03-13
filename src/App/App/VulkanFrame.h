#ifndef WX_VULKAN_FRAME_H
#define WX_VULKAN_FRAME_H

#include <wx/wx.h>
#include "VulkanRenderEngine.h"
#include "RenderPanel.h"

class wxVulkanFrame : public wxFrame
{
public:
    wxVulkanFrame(const wxString &title);
    void Cleanup();
    void Render();

    RenderPanel *renderPanel;
private:
    void OnSize(wxSizeEvent &evt);

    void OnButtonClick(wxCommandEvent &evt); // Ajoutez cette ligne

    wxDECLARE_EVENT_TABLE();
};

#endif // WX_VULKAN_FRAME_H
