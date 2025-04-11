#include "VulkanRenderEngine.h"
#include <sstream>
#include <array>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "VulkanRenderPassManager.h"
#include "VulkanRenderImageManager.h"
#include "VulkanTransferManager.h"


#include "Logger.h"


bool VulkanRenderEngine::Init(void *windowHandle)
{
    createInstance();
    VulkanDeviceManager::GetInstance().InitDeviceManager(instance);

    createDescriptorPool();
    pipelineManager = new VulkanPipelineManager(GetDeviceManager()->GetDeviceChecked());

    PassesInfo passesInfo;
    passesInfo.colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
    passesInfo.depthStencilFormat = VK_FORMAT_D24_UNORM_S8_UINT;
    passesInfo.shadowMapFormat = VK_FORMAT_D24_UNORM_S8_UINT;
    passesInfo.hdrFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

    VulkanRenderPassManager::GetInstance()->InitRenderPasses(
        GetDeviceManager()->GetDeviceChecked(),
        passesInfo
    );

    VulkanRenderImageManager::GetInstance()->Init(GetVulkanDeviceManager().GetDeviceChecked(), GetVulkanDeviceManager().GetPhysicalDeviceChecked());
    GetVulkanTransferManager().InitTransfereManager();

    return true;
}

void VulkanRenderEngine::Shutdown() 
{
    if (GetVulkanDeviceManager().GetDevice() != VK_NULL_HANDLE) 
        vkDeviceWaitIdle(GetVulkanDeviceManager().GetDevice());

    VulkanRenderPassManager::GetInstance()->Cleanup();
    GetVulkanTransferManager().Shutdown();

    if (descriptorPoolManager)
        delete descriptorPoolManager;

    delete pipelineManager;
    VulkanDeviceManager::GetInstance().Shutdown();

    if (instance != VK_NULL_HANDLE) 
    {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) 
{
    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE; // On ne stoppe pas l'exécution du programme
}

void VulkanRenderEngine::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan wxWidgets App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char *> extensions;
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback; // Fonction de callback

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

    if (auto result = vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        Log(Critical, "Vulkan", "Failed to create instance, result code : %d", result);
        throw std::runtime_error("failed to create Vulkan instance");
    }
}

void VulkanRenderEngine::createDescriptorPool()
{
    descriptorPoolManager = new VulkanDescriptorPoolManager(this);
}