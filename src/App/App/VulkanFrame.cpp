#include "VulkanFrame.h"

wxBEGIN_EVENT_TABLE(wxVulkanFrame, wxFrame)
    EVT_BUTTON(wxID_ANY, wxVulkanFrame::OnButtonClick) // Ajoutez cette ligne
wxEND_EVENT_TABLE()

wxVulkanFrame::wxVulkanFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(720, 480))
{
    wxPanel* mainPanel = new wxPanel(this);
    
    // Création d'un panneau pour le rendu
    renderPanel = new VulkanPanel(mainPanel); // Ajuster la position et la taille
    renderPanel->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    
    wxButton *button = new wxButton(mainPanel, wxID_ANY, "Test", wxPoint(10, 10), wxSize(90, 30));
    button->Bind(wxEVT_BUTTON, &wxVulkanFrame::OnButtonClick, this);
    button->Raise();
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(renderPanel, 1, wxEXPAND);
    mainPanel->SetSizer(sizer);
}

void wxVulkanFrame::Cleanup()
{
    renderPanel->Cleanup();
}

void wxVulkanFrame::Render()
{
    renderPanel->Render();
}

void wxVulkanFrame::OnButtonClick(wxCommandEvent &evt)
{
    try
    {
        float red = renderPanel->renderer->getClearColor()[0];
        float green = renderPanel->renderer->getClearColor()[1];
        float blue = renderPanel->renderer->getClearColor()[2];

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

        renderPanel->renderer->setClearColor(red, green, blue, renderPanel->renderer->getClearColor()[3]);
    }
    catch (const std::exception &e)
    {
        wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
    }
}
