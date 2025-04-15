#ifndef VULKANAPIMODULE_H
#define VULKANAPIMODULE_H

#include <vulkan/vulkan.h>

//TODO: implement base module interface
class VulkanAPIModule
{
private:
    // Singleton pattern to ensure only one instance of VulkanAPIModule exists
    VulkanAPIModule() = default;
    VulkanAPIModule(const VulkanAPIModule&) = delete;
    VulkanAPIModule& operator=(const VulkanAPIModule&) = delete;
    VulkanAPIModule(VulkanAPIModule&&) = delete;
    VulkanAPIModule& operator=(VulkanAPIModule&&) = delete;
    virtual ~VulkanAPIModule() { ShutdownModule(); }

public:
    
    /**
     * @brief Get the singleton instance of VulkanAPIModule
     * @return Reference to the VulkanAPIModule instance
     */
    static VulkanAPIModule& GetInstance()
    {
        static VulkanAPIModule instance;
        return instance;
    }

    /**
     * @brief Initialize the Vulkan API module
     */
    void InitModule();

    /**
     * @brief Shutdown the Vulkan API module
     */
    void ShutdownModule();

    /**
     * @brief Get the Vulkan instance
     * @return The Vulkan instance handle
     */
    VkInstance GetVulkanInstance() { return vulkanInstance; }

private:
    /**
     * @brief Create the Vulkan instance
     * @throws std::runtime_error if instance creation fails
     */
    void CreateInstance();

private:
    VkInstance vulkanInstance{ VK_NULL_HANDLE };
    VkDebugUtilsMessengerEXT debugMessenger{ VK_NULL_HANDLE };
};

/**
 * @brief Get the VulkanAPIModule instance
 * @return Reference to the VulkanAPIModule instance
 */
inline VulkanAPIModule& GetVulkanAPIModule()
{
    return VulkanAPIModule::GetInstance();
}

#endif // VULKANAPIMODULE_H
