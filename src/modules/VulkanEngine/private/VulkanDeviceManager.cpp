#include "VulkanDeviceManager.h"
#include <stdexcept>  
#include <vector>  

VulkanDeviceManager::VulkanDeviceManager(VkInstance instance)
{
    PickPhysicalDevice(instance);
    CreateLogicalDevice();
}

VulkanDeviceManager::~VulkanDeviceManager()
{
    if (device != VK_NULL_HANDLE) 
    {
        vkDestroyDevice(device, nullptr);
    }
}

void VulkanDeviceManager::PickPhysicalDevice(VkInstance instance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (deviceCount == 0) 
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) 
    {
        // Récupère les familles de queues
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        bool hasGraphics = false, hasTransfer = false, hasCompute = false, hasPresent = false;

        for (uint32_t i = 0; i < queueFamilyCount; i++) 
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            {
                graphicsQueueFamilyIndex = i;
                hasGraphics = true;
            }

            if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) // Idéalement une queue transfert dédiée
            {
                transferQueueFamilyIndex = i;
                hasTransfer = true;
            }
        }

        if (hasGraphics && hasTransfer) 
        {
            physicalDevice = device;
            return;
        }
    }

    throw std::runtime_error("Failed to find a suitable GPU!");
}

void VulkanDeviceManager::CreateLogicalDevice()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::vector<uint32_t> uniqueQueueFamilies = { graphicsQueueFamilyIndex, transferQueueFamilyIndex };

    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) 
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = deviceExtensions;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(device, transferQueueFamilyIndex, 0, &transferQueue);
}
