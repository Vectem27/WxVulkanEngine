#ifndef VULKANDEVICEMANAGER_H
#define VULKANDEVICEMANAGER_H

#include <vulkan/vulkan.h>

/** 
 * @brief The VulkanDeviceManager class is a singleton that manages the Vulkan device and its resources.
 * It is responsible for creating and destroying the Vulkan device, as well as managing the graphics and transfer queues.
 * @class VulkanDeviceManager
 * 
 * @author Vectem
 */
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
    
    /** 
     * @brief Initializes the Vulkan device manager. This function should be called after the Vulkan instance is created.
     * @param instance The Vulkan instance to use for creating the device manager.
     */
    void Init(VkInstance instance);

    /** 
     * @brief Cleans up the Vulkan device manager. This function should be called before destroying the Vulkan instance.
     * @param instance The Vulkan instance to use for destroying the device manager.
     */
    void Cleanup();
public:
    /** 
     * @brief Returns the Vulkan device manager instance.
     * @return The Vulkan device manager instance.
     */
    VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }

    /** 
     * @brief Returns the Vulkan device manager instance, checking for errors.
     * @return The Vulkan device manager instance.
     * @throws std::runtime_error if the device is not initialized.
     */
    VkPhysicalDevice GetPhysicalDeviceChecked() const;

    /** 
     * @brief Returns the Vulkan device.
     * @return The Vulkan device.
     */
    VkDevice GetDevice() const { return device; }

    /** 
     * @brief Returns the Vulkan device, checking for errors.
     * @return The Vulkan device.
     * @throws std::runtime_error if the device is not initialized.
     */
    VkDevice GetDeviceChecked() const;

    /** 
     * @brief Returns the graphics queue.
     * @return The graphics queue.
     */
    VkQueue GetGraphicsQueue() const { return graphicsQueue; }

    /** 
     * @brief Returns the graphics queue, checking for errors.
     * @return The graphics queue.
     */
    VkQueue GetTransferQueue() const { return transferQueue; }

    /** 
     * @brief Returns the graphics queue family index.
     * @return The graphics queue family index.
     */
    uint32_t GetGraphicsQueueFamilyIndex() const { return graphicsQueueFamilyIndex; }

    /** 
     * @brief Returns the transfer queue family index.
     * @return The transfer queue family index.
     */
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