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

class VulkanRenderEngine : public IRenderEngine
{
public:
    ~VulkanRenderEngine() { Shutdown(); }

    virtual bool Init(void* windowHandle) override;
    void render();
    virtual void Shutdown() override;

    VkInstance GetInstance() { return instance; }

    VkQueue GetPresentQueue() const { return surfaceTest->GetPresentQueue(); }
    //[[deprecated]]
    VkQueue GetGraphicsQueue() const { return GetVulkanDeviceManager().GetGraphicsQueue(); }
    
    VulkanSurface* GetSurfaceTest() const { return surfaceTest; }
    //[[deprecated]]
    VulkanDeviceManager* GetDeviceManager() const { return &GetVulkanDeviceManager(); }
    VulkanDescriptorPoolManager* GetDescriptorPoolManager() { return descriptorPoolManager;}

    VulkanPipelineManager* GetPipelineManager() { return &VulkanPipelineManager::GetInstance(); }
private:
    VkInstance instance{ VK_NULL_HANDLE };
    VulkanSurface* surfaceTest{ nullptr };
    
    VulkanDescriptorPoolManager* descriptorPoolManager;

    VulkanPipelineManager* pipelineManager;
private:
    void createInstance();
    void createDescriptorPool();

public:
    VkDebugUtilsMessengerEXT debugMessenger;
};

#endif // VULKAN_RENDERER_H
