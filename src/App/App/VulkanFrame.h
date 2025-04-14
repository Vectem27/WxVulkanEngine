#ifndef WX_VULKAN_FRAME_H
#define WX_VULKAN_FRAME_H

#include <wx/wx.h>
#include "VulkanAPIModule.h"
#include "RenderSurface.h"

#include <functional>
class wxVulkanFrame : public wxFrame
{
public:
    wxVulkanFrame(const wxString &title);

    RenderSurface *renderSurface;
    wxPanel* testPanel;
    wxPanel* mainPanel;

    void SetSliderChangeCallback(std::function<void(int)> callback) { sliderCallback = callback; }
    void SetSlider2ChangeCallback(std::function<void(int)> callback) { slider2Callback = callback; }

private:
    void OnSize(wxSizeEvent &evt);

    void OnButtonClick(wxCommandEvent &evt); // Ajoutez cette ligne

    void OnSliderChanged(wxCommandEvent& event)
    {
        if (sliderCallback)
            sliderCallback(event.GetInt());
    }

    void OnSlider2Changed(wxCommandEvent& event)
    {
        if (slider2Callback)
            slider2Callback(event.GetInt());
    }
    

    std::function<void(int)> sliderCallback;
    std::function<void(int)> slider2Callback;

    wxDECLARE_EVENT_TABLE();
};

#endif // WX_VULKAN_FRAME_H
