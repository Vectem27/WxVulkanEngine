#include "VulkanFrame.h"

wxBEGIN_EVENT_TABLE(wxVulkanFrame, wxFrame)
    EVT_SIZE(wxVulkanFrame::OnSize)
    EVT_BUTTON(wxID_ANY, wxVulkanFrame::OnButtonClick)  // Ajoutez cette ligne
wxEND_EVENT_TABLE()

wxVulkanFrame::wxVulkanFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 630)), 
      rendererInitialized(false)
{
    // Création d'un panneau pour le rendu
    renderPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 30), wxSize(800, 600)); // Ajuster la position et la taille
    renderPanel->SetBackgroundStyle(wxBG_STYLE_PAINT);

    // On associe l'événement de peinture au panel
    renderPanel->Bind(wxEVT_PAINT, &wxVulkanFrame::OnPaint, this);

    // Création du bouton en haut à droite (enfant de la fenêtre principale)
    wxButton* button = new wxButton(this, wxID_ANY, "Render", wxPoint(0, 0), wxSize(90, 30));
    button->Bind(wxEVT_BUTTON, &wxVulkanFrame::OnButtonClick, this);

    renderer = new VulkanRenderer();

    if (!rendererInitialized) 
    {
#ifdef __WXMSW__
        // Pour Windows, récupération du handle natif.
        renderer->init(reinterpret_cast<void*>(renderPanel->GetHWND()));
#else
        // Pour d'autres plateformes, il faudra récupérer le handle natif approprié.
        renderer->init(nullptr);
#endif
        rendererInitialized = true;
        renderer->render();
    }


}

void wxVulkanFrame::Cleanup() 
{
    if (renderer) 
    {
        renderer->cleanup();
        delete renderer;
    }
}

void wxVulkanFrame::OnSize(wxSizeEvent& evt) {
    // Dans une implémentation complète, on recréera le swapchain ici
    evt.Skip();
}

void wxVulkanFrame::OnPaint(wxPaintEvent& evt) {
    // L'événement de peinture vient du panel, donc on crée le DC sur le panel
    wxPaintDC dc(renderPanel);

    if (!rendererInitialized) 
    {
#ifdef __WXMSW__
        // Pour Windows, récupération du handle natif.
        renderer->init(reinterpret_cast<void*>(renderPanel->GetHWND()));
#else
        // Pour d'autres plateformes, il faudra récupérer le handle natif approprié.
        renderer->init(nullptr);
#endif
        rendererInitialized = true;
    }

    try 
    {
        renderer->render();
    } 
    catch (const std::exception& e) 
    {
        wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
    }

    evt.Skip();
}

void wxVulkanFrame::OnButtonClick(wxCommandEvent& evt)
{
    try 
    {
        float red = renderer->getClearColor()[0];
        float green = renderer->getClearColor()[1];
        float blue = renderer->getClearColor()[2];
        
        if (blue < 1.0f)
            blue += 0.1f;
        else if (green < 1.0f)
            green += 0.1f;
        else if (red < 1.0f)
            red += 0.1f;
        else
            wxMessageBox("White screen", "Info", wxICON_INFORMATION);

        red = red > 1.0f ? 1.0f : red;
        green = green > 1.0f ? 1.0f : green;
        blue = blue > 1.0f ? 1.0f : blue;
        

        renderer->setClearColor(red, green, blue, renderer->getClearColor()[3]);
        renderer->render();
    } 
    catch (const std::exception& e) 
    {
        wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
    }
}
