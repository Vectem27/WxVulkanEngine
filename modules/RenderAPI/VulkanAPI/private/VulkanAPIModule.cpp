#include "VulkanAPIModule.h"

#include <stdexcept>

#include "Logger.h"

#include "VulkanRenderPassManager.h"
#include "VulkanTransferManager.h"
#include "VulkanDeviceManager.h"

#include "VulkanRenderTargetRenderer.h"
#include "VulkanShadowMapRenderer.h"

#ifdef _WIN32
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(__linux__)
#include <vulkan/vulkan_xcb.h> // Remplace par xlib ou wayland si nécessaire
#endif

void VulkanAPIModule::InitModule()
{
    CreateInstance();

    GetVulkanDeviceManager().Init(vulkanInstance);

    PassesInfo passesInfo;
    passesInfo.colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
    passesInfo.depthStencilFormat = VK_FORMAT_D24_UNORM_S8_UINT;
    passesInfo.depthNoStencilFormat = VK_FORMAT_D32_SFLOAT;
    passesInfo.hdrFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

    GetVulkanRenderPassManager().InitRenderPasses(
        GetVulkanDeviceManager().GetDeviceChecked(),
        passesInfo
    );

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
}

void VulkanAPIModule::ShutdownModule() 
{
    if (vulkanInstance == VK_NULL_HANDLE) 
        return;

    vkDeviceWaitIdle(GetVulkanDeviceManager().GetDeviceChecked());

    GetVulkanShadowMapRenderer().Shutdown();
    GetVulkanRenderTargetRenderer().Shutdown();


    GetVulkanRenderPassManager().Cleanup();
    GetVulkanTransferManager().Shutdown();

    VulkanDeviceManager::GetInstance().Cleanup();

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
    //TODO: Remove this line
    // Temp deactivate debug callback
    return VK_FALSE;

    LogLevel level;
    if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) 
        level = Debug;
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) 
        level = Info;
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) 
        level = Warning;
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) 
        level = Error;
    else 
        level = Trace;

    std::string message;

    if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) 
        message = "General message : %s";
    else if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) 
        message = "Validation message : %s";
    else if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) 
        message = "Performance message : %s";
    else if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT) 
        message = "Device address binding message : %s";
    else
        message = "Unknown message : %s";      

    Log(level, "VulkanDebugExtLog", message.c_str(), pCallbackData->pMessage);
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
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__linux__)
    extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    // Structure de debug messenger
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{}; 
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback; // Fonction de callback

    VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures{}; 
    indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
    indexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
    indexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
    indexingFeatures.runtimeDescriptorArray = VK_TRUE;

    indexingFeatures.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

    VkInstanceCreateInfo createInfo{}; 
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.pNext = (VkPhysicalDeviceDescriptorIndexingFeatures*)&indexingFeatures;

    if (auto result = vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS)
    {
        Log(Critical, "Vulkan", "Failed to create instance, result code : %d", result);
        throw std::runtime_error("failed to create Vulkan instance");
    }

    // Setup Debug Messenger
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
