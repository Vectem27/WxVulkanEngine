#ifndef VULKANTRANSFEREMANAGER_H
#define VULKANTRANSFEREMANAGER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <mutex>


class VulkanTransferManager
{
private:
    VulkanTransferManager() = default;
    ~VulkanTransferManager() { Shutdown(); }

    void InitTransfereManager();
    void Shutdown();

    friend class VulkanAPIModule;
public:
    VulkanTransferManager(const VulkanTransferManager&) = delete;
    VulkanTransferManager& operator=(const VulkanTransferManager&) = delete;

    static VulkanTransferManager& GetInstance() 
    {
        static VulkanTransferManager instance;
        return instance;
    }

    void CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

private:
    std::mutex mutex;
    VkCommandPool commandPool{VK_NULL_HANDLE};
    VkQueue transferQueue{VK_NULL_HANDLE};
};

inline VulkanTransferManager& GetVulkanTransferManager() { return VulkanTransferManager::GetInstance(); }

#endif // VULKANTRANSFEREMANAGER_H