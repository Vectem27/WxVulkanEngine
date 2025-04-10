#ifndef VULKANRENDERPASSMANAGER_H
#define VULKANRENDERPASSMANAGER_H

#include <vulkan/vulkan.h>

class VulkanRenderEngine;

struct PassesInfo
{
    VkFormat colorFormat{VK_FORMAT_R8G8B8A8_UNORM};
    VkFormat depthStencilFormat{VK_FORMAT_D24_UNORM_S8_UINT};

    VkFormat shadowMapFormat{VK_FORMAT_D24_UNORM_S8_UINT};
    VkFormat hdrFormat{VK_FORMAT_R16G16B16A16_SFLOAT};
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
    VkFormat GetHDRFormat() const { return passesInfo.hdrFormat; }

    VkRenderPass GetGeometryPass() const { return geometryPass; }
    VkRenderPass GetShadowPass() const { return shadowPass; }
    VkRenderPass GetLightingPass() const { return lightingPass; }
    VkRenderPass GetPostprocessPass() const { return postprocessPass; }

    void InitRenderPasses(VkDevice device, PassesInfo infos);
    void Cleanup();

private:
    void InitGeometryPass(VkDevice device);
    void InitShadowPass(VkDevice device);
    void InitLightingPass(VkDevice device);
    void InitPostprocessPass(VkDevice device);

    static bool FormatContainStencil(VkFormat format);
private:
    VkDevice device{VK_NULL_HANDLE};

    VkRenderPass geometryPass{VK_NULL_HANDLE};
    VkRenderPass shadowPass{VK_NULL_HANDLE};
    VkRenderPass lightingPass{VK_NULL_HANDLE};
    VkRenderPass postprocessPass{VK_NULL_HANDLE};

    PassesInfo passesInfo;
    
    static VulkanRenderPassManager* instance;
};



#endif // VULKANRENDERPASSMANAGER_H