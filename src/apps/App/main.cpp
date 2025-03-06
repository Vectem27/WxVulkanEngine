#include <wx/wx.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

class MyFrame : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Vulkan + wxWidgets") {
        // Créez un panel pour contenir le rendu Vulkan
        wxPanel* panel = new wxPanel(this, wxID_ANY);

        // Récupérez le HWND de la fenêtre wxWidgets
        HWND hwnd = (HWND)panel->GetHandle();

        // Initialisez Vulkan
        VkInstance instance;
        VkSurfaceKHR surface;
        createVulkanInstance(instance);
        createVulkanSurface(instance, hwnd, surface);

        // Configurez la swap chain et le rendu
        initVulkanRender(instance, surface);
    }

private:
    void createVulkanInstance(VkInstance& instance) {
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

        vkCreateInstance(&createInfo, nullptr, &instance);
    }

    void createVulkanSurface(VkInstance instance, HWND hwnd, VkSurfaceKHR& surface) {
        VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = hwnd;
        createInfo.hinstance = GetModuleHandle(nullptr);

        vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
    }

    void initVulkanRender(VkInstance instance, VkSurfaceKHR surface) {
        // Configurez le dispositif physique, la swap chain, etc.
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);