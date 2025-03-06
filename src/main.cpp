#include <wx/wx.h>
#include <vulkan/vulkan.h>
#include <wxlua/wxlua.h>
#include <stdexcept>
#include "wxlua/wxlstate.h"

#include "wxbind/include/wxbinddefs.h"
WXLUA_DECLARE_BIND_WXCORE
WXLUA_DECLARE_BIND_WXBASE



class MyApp : public wxApp {
public:
    virtual bool OnInit();

private:
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);
    ~MyFrame();

private:
    VkInstance vulkanInstance;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() 
{

    WXLUA_IMPLEMENT_BIND_WXCORE
    WXLUA_IMPLEMENT_BIND_WXBASE
    MyFrame *frame = new MyFrame("Vulkan wxWidgets App");
    frame->Show(true);

    wxLuaState luaState(true);
    if (!luaState.Ok()) {
        wxMessageBox("Échec de l'initialisation de wxLua.", "Erreur", wxOK | wxICON_ERROR);
        return false;
    }

    try 
    {
        if (int rc = luaState.RunFile(wxT("scripts/script.lua"))) {
            wxMessageBox(wxlua_LUA_ERR_msg(rc), "Erreur", wxOK | wxICON_ERROR);
            return false;
        }
    }
    catch( ... )
    {
        wxMessageBox("Catch error", "Erreur", wxOK | wxICON_ERROR);
    }
    

    return true;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title), vulkanInstance(VK_NULL_HANDLE) {
    // Initialisation de Vulkan
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan wxWidgets App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS) {
        wxLogError("Failed to create Vulkan instance!");
        return;
    }
}

MyFrame::~MyFrame() {
    if (vulkanInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(vulkanInstance, nullptr);
    }
}