#include <wx/wx.h>
#include <wxlua/wxlua.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    wxLuaState m_wxlState;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MyFrame *frame = new MyFrame("wxLua Example");
    frame->Show(true);

    return true;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title), m_wxlState(true)
{
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "&Quit\tCtrl+Q", "Quit the application");

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MyFrame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);

    // Initialise wxLua
    if (m_wxlState.Ok())
    {
        m_wxlState.RunFile("scripts/script.lua");
    }
    else
    {
        wxMessageBox("Failed to initialise wxLua", "Error", wxOK | wxICON_ERROR, this);
    }
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("This is a wxLua example", "About wxLua", wxOK | wxICON_INFORMATION, this);
}