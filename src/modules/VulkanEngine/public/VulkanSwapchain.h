#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>

struct SwapchainSupportDetails 
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain
{
public:
    VulkanSwapchain(class VulkanRenderEngine* renderEngine, VkSurfaceKHR surface, uint32_t graphicsQueueFamilyIndex);
    ~VulkanSwapchain();

    void Create(VkRenderPass renderPass);
    void SetRenderPass(VkRenderPass renderPass);
    void Recreate();

    VkSwapchainKHR GetSwapchain() const { return swapchain; }
    VkExtent2D GetExtent() const { return swapchainExtent; }
    const VkFramebuffer& GetFrameBuffer(uint32_t index) const { return framebuffers[index]; }
    const VkCommandBuffer& GetCommandBuffer(uint32_t index) const { return commandBuffers[index]; }
    const VkFence& GetInFlightFence() const {return inFlightFence; }
    const VkSemaphore& GetImageAvailableSemaphore() const { return imageAvailableSemaphore; }
    const VkSemaphore& GetRenderFinishedSemaphore() const { return renderFinishedSemaphore; }

    int32_t GetImageCount() const { return imageCount; }

private:
    void CreateCommandPool(uint32_t graphicsQueueFamilyIndex);
    void CreateCommandBuffers();
    void CreateSync();

    void CreateSwapchain();
    void CreateImageView();
    void CreateFramebuffer();
    void CreateDepthResources();


    void Cleanup();

private:
    class VulkanRenderEngine* renderEngine;
    VkSurfaceKHR surface;

    VkRenderPass renderPass;

    uint32_t width{ 720 }, height{ 480 }, imageCount{ 0 };

    VkSwapchainKHR swapchain{VK_NULL_HANDLE};
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffers;

    // Depth/stencil buffer
    std::vector<VkImage> depthImages;
    std::vector<VkDeviceMemory> depthImageMemorys;
    std::vector<VkImageView> depthImageViews;
    std::vector<VkFramebuffer> depthFramebuffers;

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