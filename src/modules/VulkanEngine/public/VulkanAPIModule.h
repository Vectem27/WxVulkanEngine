#ifndef VULKANAPIMODULE_H
#define VULKANAPIMODULE_H

#include "IRenderEngine.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "IRenderEngine.h"
#include "VulkanDeviceManager.h"
#include "VulkanSurface.h"

#include "Pipeline/VulkanPipelineManager.h"

//TODO: implement base module interface

class VulkanAPIModule
{
private:
    // Singleton pattern to ensure only one instance of VulkanAPIModule exists
    VulkanAPIModule() = default;
    VulkanAPIModule(const VulkanAPIModule&) = delete;
    VulkanAPIModule& operator=(const VulkanAPIModule&) = delete;
    VulkanAPIModule(VulkanAPIModule&&) = delete;
    VulkanAPIModule& operator=(VulkanAPIModule&&) = delete;
    virtual ~VulkanAPIModule() { ShutdownModule(); }
public:
    static VulkanAPIModule& GetInstance()
    {
        static VulkanAPIModule instance;
        return instance;
    }

    bool InitModule();
    void ShutdownModule();

    VkInstance GetVulkanInstance() { return vulkanInstance; }

private:
    void CreateInstance();

private:
    VkInstance vulkanInstance{ VK_NULL_HANDLE };
    VkDebugUtilsMessengerEXT debugMessenger{ VK_NULL_HANDLE };
};

inline VulkanAPIModule& GetVulkanAPIModule()
{
    return VulkanAPIModule::GetInstance();
}

#endif // VULKANAPIMODULE_H
