#ifndef VULKANRENDERPASSMANAGER_H
#define VULKANRENDERPASSMANAGER_H

#include <vulkan/vulkan.h>

class VulkanRenderEngine;

class VulkanRenderPassManager
{
private:
    VulkanRenderPassManager() = default;
    void InitRenderPasses(VkDevice device);

    static VulkanRenderPassManager* instance;

    void InitGeometryPass(VkDevice device);
    void InitShadowPass(VkDevice device);
public:
    static VulkanRenderPassManager* GetInstance(VkDevice device);

    VkRenderPass GetGeometryPass() const { return geometryPass; }
    VkRenderPass GetShadowPass() const { return shadowPass; }

private:
    VulkanRenderEngine* vulkanRenderEngine{nullptr};

    VkRenderPass geometryPass{VK_NULL_HANDLE};
    VkRenderPass shadowPass{VK_NULL_HANDLE};

    const VkFormat colorFormat{VK_FORMAT_R8G8B8A8_UNORM};
    const VkFormat depthFormat{VK_FORMAT_D24_UNORM_S8_UINT};

    const VkFormat shadowMapFormat{VK_FORMAT_D24_UNORM_S8_UINT};
};



#endif // VULKANRENDERPASSMANAGER_H