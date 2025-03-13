#ifndef VULKANDEVICEMANAGER_H
#define VULKANDEVICEMANAGER_H

#include <vulkan/vulkan.h>

class VulkanDeviceManager
{
public:
    VulkanDeviceManager(VkInstance instance);
    ~VulkanDeviceManager();

    VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
    VkDevice GetDevice() const { return device; }
    VkQueue GetGraphicsQueue() const { return graphicsQueue; }
    VkQueue GetTransferQueue() const { return transferQueue; }
    uint32_t GetGraphicsQueueFamilyIndex() const { return graphicsQueueFamilyIndex; }
    uint32_t GetTransferQueueFamilyIndex() const { return transferQueueFamilyIndex; }

private:
    void PickPhysicalDevice(VkInstance instance);
    void CreateLogicalDevice();

    VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
    VkDevice device{ VK_NULL_HANDLE };

    VkQueue graphicsQueue{ VK_NULL_HANDLE };
    VkQueue transferQueue{ VK_NULL_HANDLE };

    uint32_t graphicsQueueFamilyIndex{ UINT32_MAX };
    uint32_t transferQueueFamilyIndex{ UINT32_MAX };
};

#endif // VULKANDEVICEMANAGER_H