#ifndef SWAPCHAINRENDERER_H
#define SWAPCHAINRENDERER_H

#include <vulkan/vulkan.h>
#include <vector>

class SwapchainRenderer
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

    uint32_t width { 720 };
    uint32_t height { 480 };

    // Sync
    VkSemaphore imageAvailableSemaphore; // Wait the image is unused befor render in it
    VkSemaphore renderFinishedSemaphore; // Wait for render finished before present
    VkFence inFlightFence; // Wait for the last rendering finished

    // Rendering
    uint32_t imageIndex; // Current rendered image index

public:
    SwapchainRenderer(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, 
        VkFormat swapChainImageFormat, uint32_t graphicsQueueFamilyIndex);
    ~SwapchainRenderer();

    VkCommandBuffer BeginRendeCommands();
    void EndRenderCommandsAndPresent(VkQueue graphicsQueue);
    void Present();

private:
    void CreateSwapChain();

    void CreateImageViews();

    void CreateFramebuffers();

    void CreateCommandPool();

    void CreateCommandBuffers();

    void CreateSync();

    void RecreateSwapChain();

    void CleanupSwapchain();
};


#endif // SWAPCHAINRENDERER_H