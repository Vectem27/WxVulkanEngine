#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

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

#include "VulkanDescriptorPoolManager.h"
#include "Pipeline/VulkanPipelineManager.h"

//TODO: Rename to VulkanAPIModule

class VulkanRenderEngine
{
public:
    ~VulkanRenderEngine() { ShutdownModule(); }

    bool InitModule() ;
    void ShutdownModule() ;

    VkInstance GetVulkanInstance() { return instance; }

    [[deprecated]]
    VulkanDescriptorPoolManager* GetDescriptorPoolManager() { return descriptorPoolManager;}
private:
    VkInstance instance{ VK_NULL_HANDLE };
    
    VulkanDescriptorPoolManager* descriptorPoolManager;

    VulkanPipelineManager* pipelineManager;
private:
    void createInstance();
    void createDescriptorPool();

public:
    VkDebugUtilsMessengerEXT debugMessenger;
};

#endif // VULKAN_RENDERER_H
