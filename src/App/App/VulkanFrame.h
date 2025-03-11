#ifndef WX_VULKAN_FRAME_H
#define WX_VULKAN_FRAME_H

#include <wx/wx.h>
#include "VulkanRenderer.h"

class VulkanPanel : public wxWindow
{
public:
    VulkanPanel(wxWindow* parent) 
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
    {
        SetWindowLongPtr(GetHWND(), GWL_STYLE, GetWindowLongPtr(GetHWND(), GWL_STYLE) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        renderer = new VulkanRenderer();

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
    }

    void Cleanup()
    {
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
            renderer->render();
        }
        catch (const std::exception &e)
        {
            wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
        }
    }

public:
    VulkanRenderer *renderer;
private:
    bool rendererInitialized{false};
};



class wxVulkanFrame : public wxFrame
{
public:
    wxVulkanFrame(const wxString &title);
    void Cleanup();
    void Render();

    VulkanPanel *renderPanel;
private:
    void OnSize(wxSizeEvent &evt);

    void OnButtonClick(wxCommandEvent &evt); // Ajoutez cette ligne

    wxDECLARE_EVENT_TABLE();
};

#endif // WX_VULKAN_FRAME_H
