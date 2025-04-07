#include "VulkanFrame.h"

enum
{
    ID_Slider1 = 1001,
    ID_Slider2 = 1002,
};

wxBEGIN_EVENT_TABLE(wxVulkanFrame, wxFrame)
    EVT_BUTTON(wxID_ANY, wxVulkanFrame::OnButtonClick) // Ajoutez cette ligne
    EVT_SLIDER(ID_Slider1, wxVulkanFrame::OnSliderChanged) // Ajoutez cette ligne
    EVT_SLIDER(ID_Slider2, wxVulkanFrame::OnSlider2Changed) // Ajoutez cette ligne
wxEND_EVENT_TABLE()

wxVulkanFrame::wxVulkanFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(720, 480))
{
    mainPanel = new wxPanel(this);
    
    // Création d'un panneau pour le rendu
    renderSurface = new RenderSurface(mainPanel); // Ajuster la position et la taille
    renderSurface->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    
    testPanel = new wxPanel(mainPanel);
    SetWindowLongPtr(testPanel->GetHWND(), GWL_STYLE, GetWindowLongPtr(testPanel->GetHWND(), GWL_STYLE) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    testPanel->Raise();

    wxButton *button = new wxButton(mainPanel, wxID_ANY, "Test", wxPoint(10, 10), wxSize(90, 30));
    button->Bind(wxEVT_BUTTON, &wxVulkanFrame::OnButtonClick, this);
    button->Raise();

    wxSlider *slider = new wxSlider(mainPanel, ID_Slider1, 0, -180, 180, wxPoint(10, 40), wxSize(90, -1));
    slider->Bind(wxEVT_SLIDER, &wxVulkanFrame::OnSliderChanged, this);
    slider->Raise();

    wxSlider *slider2 = new wxSlider(mainPanel, ID_Slider2, 0, -180, 180, wxPoint(10, 80), wxSize(90, -1));
    slider2->Bind(wxEVT_SLIDER, &wxVulkanFrame::OnSlider2Changed, this);
    slider2->Raise();


    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(renderSurface, 3, wxEXPAND);
    sizer->Add(testPanel, 1, wxEXPAND);
    mainPanel->SetSizer(sizer);
}

void wxVulkanFrame::OnButtonClick(wxCommandEvent &evt)
{
    try
    {
        
    }
    catch (const std::exception &e)
    {
        wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
    }
}
