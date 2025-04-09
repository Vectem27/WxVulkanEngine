#ifndef VULKANRENDERPASSMANAGER_H
#define VULKANRENDERPASSMANAGER_H

#include <vulkan/vulkan.h>

class VulkanRenderEngine;

struct PassesInfo
{
    VkFormat colorFormat{VK_FORMAT_R8G8B8A8_UNORM};
    VkFormat depthStencilFormat{VK_FORMAT_D24_UNORM_S8_UINT};

    VkFormat shadowMapFormat{VK_FORMAT_D24_UNORM_S8_UINT};
};

class VulkanRenderPassManager
{
private:
    VulkanRenderPassManager() = default;
    
public:
    static VulkanRenderPassManager* GetInstance();

    VkFormat GetColorFormat() const { return passesInfo.colorFormat; }
    VkFormat GetDepthStencilFormat() const { return passesInfo.depthStencilFormat; }
    VkFormat GetShadowMapFormat() const { return passesInfo.shadowMapFormat; }

    VkRenderPass GetGeometryPass() const { return geometryPass; }
    VkRenderPass GetShadowPass() const { return shadowPass; }
    VkRenderPass GetLightingPass() const { return lightingPass; }

    void InitRenderPasses(VkDevice device, PassesInfo infos);
    void Cleanup();

    void CreateDepthStencilImage(VkImage &depthImage, VkDeviceMemory& depthImageMemory) const;
    void CreateDepthStencilImageView(VkImageView &depthImageView, VkImage &depthImage) const;

private:
    void InitGeometryPass(VkDevice device);
    void InitShadowPass(VkDevice device);
    void InitLightingPass(VkDevice device);

    static bool FormatContainStencil(VkFormat format);
private:
    VkDevice device{VK_NULL_HANDLE};

    VkRenderPass geometryPass{VK_NULL_HANDLE};
    VkRenderPass shadowPass{VK_NULL_HANDLE};
    VkRenderPass lightingPass{VK_NULL_HANDLE};

    PassesInfo passesInfo;
    
    static VulkanRenderPassManager* instance;
};



#endif // VULKANRENDERPASSMANAGER_H