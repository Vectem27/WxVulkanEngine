#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include <vector>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "IRenderTarget.h"
#include "IVulkanRenderTarget.h"
#include "VulkanRenderTarget.h"

enum BufferType
{
    BASECOLOR,
    DEPTHSTENCIL,
    NORMAL,
    LIGHTING,
    POSITION,
    POSTPROCESS,
};

struct ImageData
{
    BufferType type;
    VkFormat format;
    VkImageUsageFlags usageFlags;
    VkImageAspectFlags aspectFlags;
    std::vector<VkImage> images;
    std::vector<VkDeviceMemory> imageMemorys;
    std::vector<VkImageView> imageViews;

    void Resize(uint32_t size)
    {
        images.clear();
        imageViews.clear();
        imageMemorys.clear();

        images.resize(size);
        imageMemorys.resize(size);
        imageViews.resize(size);

        for (size_t i = 0; i < size; i++)
        {
            images[i] = VK_NULL_HANDLE;
            imageMemorys[i] = VK_NULL_HANDLE;
            imageViews[i] = VK_NULL_HANDLE;
        }
    }

    void Cleanup(VkDevice device, bool excludeImages)
    {
        for (size_t i = 0; i < images.size(); i++)
        {
            if (images[i] != VK_NULL_HANDLE && !excludeImages)
                vkDestroyImage(device, images[i], nullptr);
            if (imageMemorys[i] != VK_NULL_HANDLE)
                vkFreeMemory(device, imageMemorys[i], nullptr);
            if (imageViews[i] != VK_NULL_HANDLE)
                vkDestroyImageView(device, imageViews[i], nullptr);
        }
        images.clear();
        imageViews.clear();
        imageMemorys.clear();
    }
};

struct SwapchainSupportDetails 
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain : public IRenderTarget, public IVulkanRenderTarget
{
public:
    VulkanSwapchain(class VulkanRenderEngine* renderEngine, class VulkanSurface* surface);
    ~VulkanSwapchain();

public:
    virtual uint32_t GetWidth() const override { return GetExtent().width; }
    virtual uint32_t GetHeight() const override { return GetExtent().height; }

    virtual void StartRendering() override;
    virtual VkFramebuffer GetGeometryFrameBuffer() const override { return renderTargets[renderingImageIndex].GetGeometryFrameBuffer(); }
    virtual VkFramebuffer GetLightingFrameBuffer() const override { return renderTargets[renderingImageIndex].GetLightingFrameBuffer(); }
    virtual VkFramebuffer GetPostprocessFrameBuffer() const override { return renderTargets[renderingImageIndex].GetPostprocessFrameBuffer(); }

public:
    void Create(VkRenderPass renderPass);
    void SetRenderPass(VkRenderPass renderPass);
    void Recreate();
    void Present();

    VkSwapchainKHR GetSwapchain() const { return swapchain; }
    VkExtent2D GetExtent() const { return swapchainExtent; }

    const VkCommandBuffer& GetCommandBuffer() const { return commandBuffers[renderingImageIndex]; }

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
    class VulkanRenderEngine* renderEngine;
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
    std::vector<VkCommandBuffer> commandBuffers;
    
    // Sync
    VkFence inFlightFence{ VK_NULL_HANDLE }; // Wait for the last rendering finished
    VkSemaphore imageAvailableSemaphore{ VK_NULL_HANDLE }; // Wait the image is unused befor render in it
    VkSemaphore renderFinishedSemaphore{ VK_NULL_HANDLE }; // Wait for render finished before present
};

#endif // VULKANSWAPCHAIN_H