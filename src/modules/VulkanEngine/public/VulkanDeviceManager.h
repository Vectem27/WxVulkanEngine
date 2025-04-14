#ifndef VULKANDEVICEMANAGER_H
#define VULKANDEVICEMANAGER_H

#include <vulkan/vulkan.h>

class VulkanDeviceManager
{
private:
    VulkanDeviceManager() = default;
    ~VulkanDeviceManager() = default;
    VulkanDeviceManager(VulkanDeviceManager&) = delete;
    VulkanDeviceManager& operator=(VulkanDeviceManager&) = delete;
    VulkanDeviceManager(VulkanDeviceManager&&) = delete;
    VulkanDeviceManager& operator=(VulkanDeviceManager&&) = delete;
public:
    static VulkanDeviceManager& GetInstance() 
    {
        static VulkanDeviceManager instance;  // Thread-safe after C++11
        return instance;
    }

    friend class VulkanAPIModule;
    void InitDeviceManager(VkInstance instance);
    void Shutdown();
public:
    

    VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
    VkPhysicalDevice GetPhysicalDeviceChecked() const;
    VkDevice GetDevice() const { return device; }
    VkDevice GetDeviceChecked() const;
    VkQueue GetGraphicsQueue() const { return graphicsQueue; }
    VkQueue GetTransferQueue() const { return transferQueue; }
    uint32_t GetGraphicsQueueFamilyIndex() const { return graphicsQueueFamilyIndex; }
    uint32_t GetTransferQueueFamilyIndex() const { return transferQueueFamilyIndex; }

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
private:
    void PickPhysicalDevice(VkInstance instance);
    void CreateLogicalDevice();

    VkDevice device{ VK_NULL_HANDLE };
    VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };

    VkQueue graphicsQueue{ VK_NULL_HANDLE };
    VkQueue transferQueue{ VK_NULL_HANDLE };

    uint32_t graphicsQueueFamilyIndex{ UINT32_MAX };
    uint32_t transferQueueFamilyIndex{ UINT32_MAX };
};

inline VulkanDeviceManager& GetVulkanDeviceManager() { return VulkanDeviceManager::GetInstance(); }


#endif // VULKANDEVICEMANAGER_H