#ifndef SWAPCHAINRENDERER_H
#define SWAPCHAINRENDERER_H

#include "IRenderTarget.h"
#include <vulkan/vulkan.h>
#include <vector>

class SwapchainRenderer : public IRenderTarget
{
private:
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR surface;
    VkFormat swapChainImageFormat;
    VkRenderPass renderPass;
    uint32_t graphicsQueueFamilyIndex;

    // Commands
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    // Swapchain
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;


    // Depth/stencil buffer
    std::vector<VkImage> depthImages;
    std::vector<VkDeviceMemory> depthImageMemorys;
    std::vector<VkImageView> depthImageViews;
    std::vector<VkFramebuffer> depthFramebuffers;
    VkFormat depthFormat = VK_FORMAT_D24_UNORM_S8_UINT;

public:
    uint32_t width { 720 };
    uint32_t height { 480 };

private:
    // Sync
    VkSemaphore imageAvailableSemaphore; // Wait the image is unused befor render in it
    VkSemaphore renderFinishedSemaphore; // Wait for render finished before present
    VkFence inFlightFence; // Wait for the last rendering finished

    // Rendering
    uint32_t imageIndex; // Current rendered image index

    class VulkanRenderer* renderEngine;

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
public:
    SwapchainRenderer(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, 
        VkFormat swapChainImageFormat, uint32_t graphicsQueueFamilyIndex);
    ~SwapchainRenderer();

    virtual bool Init(class IRenderEngine* renderEngine) override;
    virtual void Cleanup() override;
    virtual const VkCommandBuffer& GetCurrentCommandBuffer() const override { return commandBuffers[imageIndex]; }
    virtual uint32_t GetWidth() const override { return width; }
    virtual uint32_t GetHeight() const override { return height; }

    bool BeginRenderCommands();
    void EndRenderCommandsAndPresent(VkQueue presentQueue, VkQueue graphicsQueue);

public:
    void SetRenderPass(VkRenderPass renderPass);



private: // Initialization
    void CreateSwapChain();

    void CreateImageViews();

    void CreateFramebuffers();

    void CreateDepthResources();

    void CreateCommandPool();

    void CreateCommandBuffers();

    void CreateSync();

private: // Modification
    void RecreateSwapChain();

private: // Destruction
    void CleanupSwapchain();
};


#endif // SWAPCHAINRENDERER_H