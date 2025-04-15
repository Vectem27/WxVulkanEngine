#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include <vector>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "IVulkanRenderTarget.h"
#include "VulkanRenderTarget.h"

struct SwapchainSupportDetails 
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain :  public IVulkanRenderTarget
{
public:
    VulkanSwapchain(class VulkanSurface* surface);
    ~VulkanSwapchain();

public:
    virtual uint32_t GetWidth() const override { return GetExtent().width; }
    virtual uint32_t GetHeight() const override { return GetExtent().height; }

    virtual VkFramebuffer GetGeometryFrameBuffer() const override { return renderTargets[renderingImageIndex].GetGeometryFrameBuffer(); }
    virtual VkFramebuffer GetLightingFrameBuffer() const override { return renderTargets[renderingImageIndex].GetLightingFrameBuffer(); }
    virtual VkFramebuffer GetPostprocessFrameBuffer() const override { return renderTargets[renderingImageIndex].GetPostprocessFrameBuffer(); }

    virtual void BeginRendering(VkCommandBuffer commandBuffer) override;
    virtual void EndRendering(VkQueue queue, VkCommandBuffer commandBuffer) override;

    virtual void StartLighting(VkCommandBuffer commandBuffer) override;
    virtual void StartPostprocessing(VkCommandBuffer commandBuffer) override;
public:
    void Recreate();
    void Present();

    VkSwapchainKHR GetSwapchain() const { return swapchain; }
    VkExtent2D GetExtent() const { return swapchainExtent; }

    const VkCommandBuffer& GetCommandBuffer() const { return commandBuffer; }

    const VkFence& GetInFlightFence() const {return inFlightFence; }
    const VkSemaphore& GetImageAvailableSemaphore() const { return imageAvailableSemaphore; }
    const VkSemaphore& GetRenderFinishedSemaphore() const { return renderFinishedSemaphore; }

    int32_t GetImageCount() const { return imageCount; }

    void UpdateGBufferDescriptorSet();
    VkDescriptorSet& GetGBufferDescriptorSet() { return gBufferDescriptorSet; }

    void UpdatePostprocessDescriptorSet();
    VkDescriptorSet& GetPostprocessDescriptorSet() { return postprocessDescriptorSet; }
private:
    void CreateCommandPool(uint32_t graphicsQueueFamilyIndex);
    void CreateCommandBuffers();    
    void CreateSync();

    void CreateSwapchain();

    void Cleanup();


private:
    class VulkanAPIModule* renderEngine;
    class VulkanSurface* surface;

    uint32_t width{ 720 }, height{ 480 }, imageCount{ 0 }, renderingImageIndex{0};

    VkSwapchainKHR swapchain{VK_NULL_HANDLE};

    std::vector<VulkanRenderTarget> renderTargets;

    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;

    VkDescriptorSet gBufferDescriptorSet{VK_NULL_HANDLE};
    VkDescriptorSet postprocessDescriptorSet{VK_NULL_HANDLE};

    VkExtent2D swapchainExtent;
    
    // Commands
    VkCommandPool commandPool{VK_NULL_HANDLE};
    VkCommandBuffer commandBuffer;
    
    // Sync
    VkFence inFlightFence{ VK_NULL_HANDLE }; // Wait for the last rendering finished
    VkSemaphore imageAvailableSemaphore{ VK_NULL_HANDLE }; // Wait the image is unused befor render in it
    VkSemaphore renderFinishedSemaphore{ VK_NULL_HANDLE }; // Wait for render finished before present

    VkDescriptorPool descriptorPool{ VK_NULL_HANDLE };
};

#endif // VULKANSWAPCHAIN_H