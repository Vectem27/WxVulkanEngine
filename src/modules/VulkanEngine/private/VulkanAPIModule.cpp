#include "VulkanAPIModule.h"

#include "Logger.h"

#include "VulkanRenderPassManager.h"
#include "VulkanRenderImageManager.h"
#include "VulkanTransferManager.h"

#include "VulkanRenderTargetRenderer.h"
#include "VulkanShadowMapRenderer.h"

bool VulkanAPIModule::InitModule()
{
    CreateInstance();
    VulkanDeviceManager::GetInstance().InitDeviceManager(vulkanInstance);

    PassesInfo passesInfo;
    passesInfo.colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
    passesInfo.depthStencilFormat = VK_FORMAT_D24_UNORM_S8_UINT;
    passesInfo.shadowMapFormat = VK_FORMAT_D24_UNORM_S8_UINT;
    passesInfo.hdrFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

    VulkanRenderPassManager::GetInstance()->InitRenderPasses(
        GetVulkanDeviceManager().GetDeviceChecked(),
        passesInfo
    );

    VulkanRenderImageManager::GetInstance()->Init(GetVulkanDeviceManager().GetDeviceChecked(), GetVulkanDeviceManager().GetPhysicalDeviceChecked());
    GetVulkanTransferManager().InitTransfereManager();

    /* Init RenderTargetRenderer*/
    GetVulkanRenderTargetRenderer().Init(
        GetVulkanDeviceManager().GetGraphicsQueue(), 
        GetVulkanDeviceManager().GetGraphicsQueueFamilyIndex()
    );
    GetVulkanRenderTargetRenderer().SetGeometryRenderPass(GetVulkanRenderPassManager().GetGeometryPass());
    GetVulkanRenderTargetRenderer().SetLightingRenderPass(GetVulkanRenderPassManager().GetLightingPass());
    GetVulkanRenderTargetRenderer().SetPostprocessRenderPass(GetVulkanRenderPassManager().GetPostprocessPass());

    /* Init ShadowMapRenderer*/
    GetVulkanShadowMapRenderer().Init(
        GetVulkanDeviceManager().GetGraphicsQueue(), 
        GetVulkanDeviceManager().GetGraphicsQueueFamilyIndex()
    );
    GetVulkanShadowMapRenderer().SetRenderPass(GetVulkanRenderPassManager().GetShadowPass());
    
    return true;
}

void VulkanAPIModule::ShutdownModule() 
{
    if (vulkanInstance == VK_NULL_HANDLE) 
        return;

    vkDeviceWaitIdle(GetVulkanDeviceManager().GetDeviceChecked());

    GetVulkanShadowMapRenderer().Shutdown();
    GetVulkanRenderTargetRenderer().Shutdown();


    VulkanRenderPassManager::GetInstance()->Cleanup();
    GetVulkanTransferManager().Shutdown();

    VulkanDeviceManager::GetInstance().Shutdown();

    auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)
    vkGetInstanceProcAddr(vulkanInstance, "vkDestroyDebugUtilsMessengerEXT");
    if (destroyFunc != nullptr) 
    {
        destroyFunc(vulkanInstance, debugMessenger, nullptr);
    }

    if (vulkanInstance != VK_NULL_HANDLE) 
    {
        vkDestroyInstance(vulkanInstance, nullptr);
        vulkanInstance = VK_NULL_HANDLE;
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) 
{
    Log(Debug, "Vulkan" "Validation layer : %s", pCallbackData->pMessage);
    return VK_FALSE;
}

void VulkanAPIModule::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan wxWidgets App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vulkan API";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char *> extensions;
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__linux__)
    extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = 
        //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback; // Fonction de callback

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

    if (auto result = vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS)
    {
        Log(Critical, "Vulkan", "Failed to create instance, result code : %d", result);
        throw std::runtime_error("failed to create Vulkan instance");
    }

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
    vkGetInstanceProcAddr(vulkanInstance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        if (func(vulkanInstance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) 
        {
            Log(Warning, "Vulkan", "Failed to set up debug messenger!");
        }
    }
}