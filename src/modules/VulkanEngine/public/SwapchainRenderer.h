#ifndef SWAPCHAINRENDERER_H
#define SWAPCHAINRENDERER_H

#include "IRenderTarget.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanSwapchain.h"

class SwapchainRenderer : public IRenderTarget
{
private:
    VkSurfaceKHR surface;
    uint32_t graphicsQueueFamilyIndex;

    // Commands
    VkCommandPool commandPool{VK_NULL_HANDLE};
    std::vector<VkCommandBuffer> commandBuffers;
    
    // Sync
    VkSemaphore imageAvailableSemaphore; // Wait the image is unused befor render in it
    VkSemaphore renderFinishedSemaphore; // Wait for render finished before present
    VkFence inFlightFence; // Wait for the last rendering finished


    VulkanSwapchain* vulkanSwapchain{ nullptr };

private:
    
    // Rendering
    uint32_t imageIndex; // Current rendered image index

    class VulkanRenderEngine* renderEngine;

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
public:
    SwapchainRenderer(VkSurfaceKHR surface, uint32_t graphicsQueueFamilyIndex);
    ~SwapchainRenderer();

    virtual bool Init(class IRenderEngine* renderEngine) override;
    virtual void Cleanup() override;
    virtual const VkCommandBuffer& GetCurrentCommandBuffer() const override { return commandBuffers[imageIndex]; }
    virtual uint32_t GetWidth() const override { return vulkanSwapchain->GetExtent().width; }
    virtual uint32_t GetHeight() const override { return vulkanSwapchain->GetExtent().height; }

    bool BeginRenderCommands();
    void EndRenderCommandsAndPresent(VkQueue presentQueue, VkQueue graphicsQueue);

public:
    void SetRenderPass(VkRenderPass renderPass);

private: // Initialization
    void CreateCommandPool();

    void CreateCommandBuffers();

    void CreateSync();

private: // Modification
    void RecreateSwapChain();

private: // Destruction
    void CleanupSwapchain();
};


#endif // SWAPCHAINRENDERER_H