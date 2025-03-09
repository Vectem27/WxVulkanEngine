#include "VulkanFrame.h"

wxBEGIN_EVENT_TABLE(wxVulkanFrame, wxFrame)
    EVT_BUTTON(wxID_ANY, wxVulkanFrame::OnButtonClick) // Ajoutez cette ligne
wxEND_EVENT_TABLE()

wxVulkanFrame::wxVulkanFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 630)),
      rendererInitialized(false)
{
    wxPanel* mainPanel = new wxPanel(this);
    
    // Création d'un panneau pour le rendu
    renderPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize); // Ajuster la position et la taille
    renderPanel->SetBackgroundStyle(wxBG_STYLE_PAINT);

    // On associe l'événement de peinture au panel
    renderPanel->Bind(wxEVT_PAINT, &wxVulkanFrame::OnPaint, this);
    renderPanel->Bind(wxEVT_SIZE, &wxVulkanFrame::OnSize, this);

    // Création du bouton en haut à droite (enfant de la fenêtre principale)
    wxButton *button = new wxButton(mainPanel, wxID_ANY, "Test", wxPoint(10, 10));
    button->Bind(wxEVT_BUTTON, &wxVulkanFrame::OnButtonClick, this);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(renderPanel, 1, wxEXPAND);
    mainPanel->SetSizer(sizer);

    button->Raise();


    renderer = new VulkanRenderer();

    if (!rendererInitialized)
    {
#ifdef __WXMSW__
        // Pour Windows, récupération du handle natif.
        renderer->init(reinterpret_cast<void *>(renderPanel->GetHWND()), ViewportData({800, 600}));
#else
        // Pour d'autres plateformes, il faudra récupérer le handle natif approprié.
        renderer->init(nullptr, ViewportData({800, 600}));
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

void wxVulkanFrame::Render()
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

void wxVulkanFrame::OnSize(wxSizeEvent &evt)
{
    renderer->ViewportSize({(unsigned int)renderPanel->GetSize().GetWidth(), (unsigned int)evt.GetSize().GetHeight()});
    evt.Skip();
}

void wxVulkanFrame::OnPaint(wxPaintEvent &evt)
{
    wxPaintDC dc(renderPanel);

    evt.Skip();
}

void wxVulkanFrame::OnButtonClick(wxCommandEvent &evt)
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
    }
    catch (const std::exception &e)
    {
        wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
    }
}
