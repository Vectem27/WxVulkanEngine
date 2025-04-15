#include "VulkanDeviceManager.h"
#include <stdexcept>  
#include <vector>  
#include "Logger.h"

void VulkanDeviceManager::Init(VkInstance instance)
{
    if (instance == VK_NULL_HANDLE)
    {
        Log(Critical, "Vulkan", "Failed to init device manager, instance is null");
        throw std::invalid_argument("Failed to init device manager, instance is null");
    }

    PickPhysicalDevice(instance);
    CreateLogicalDevice();
}

void VulkanDeviceManager::Cleanup()
{
    if (device != VK_NULL_HANDLE) 
    {
        vkDestroyDevice(device, nullptr);
    }
}

VkPhysicalDevice VulkanDeviceManager::GetPhysicalDeviceChecked() const
{
    if (physicalDevice == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Trying to access to an invalid physical device");
        throw std::runtime_error("Trying to access to an invalid physical device");
    }
    return physicalDevice;
}

VkDevice VulkanDeviceManager::GetDeviceChecked() const
{
    if (device == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Trying to access to an invalid device");
        throw std::runtime_error("Trying to access to an invalid device");
    }
    return device; 
}

uint32_t VulkanDeviceManager::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(GetPhysicalDeviceChecked(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && 
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    Log(Error, "Vulkan", "Failed to find memory type");
    throw std::runtime_error("Failed to find memory type");
}

void VulkanDeviceManager::PickPhysicalDevice(VkInstance instance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (deviceCount == 0) 
    {
        Log(Critical, "Vulkan", "Failed to find GPUs with Vulkan support");
        throw std::runtime_error("Failed to find GPUs with Vulkan support");
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
    Log(Critical, "Vulkan", "Failed to find a suitable GPU");
    throw std::runtime_error("Failed to find a suitable GPU");
}

void VulkanDeviceManager::CreateLogicalDevice()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    std::vector<uint32_t> uniqueQueueFamilies;
    uniqueQueueFamilies.push_back(graphicsQueueFamilyIndex);
    if (graphicsQueueFamilyIndex != transferQueueFamilyIndex)
        uniqueQueueFamilies.push_back(transferQueueFamilyIndex);

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
    deviceFeatures.independentBlend = VK_TRUE;
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = deviceExtensions;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
    {
        Log(Critical, "Vulkan", "Failed to create logical device");
        throw std::runtime_error("Failed to create vulkan logical device");
    }

    vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(device, transferQueueFamilyIndex, 0, &transferQueue);
}
