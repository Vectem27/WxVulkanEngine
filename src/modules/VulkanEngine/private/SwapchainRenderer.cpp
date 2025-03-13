#include "SwapchainRenderer.h"
#include <stdexcept>
#include <algorithm>
#include "VulkanRenderEngine.h"
#include <array>


SwapchainRenderer::SwapchainRenderer(VkSurfaceKHR surface, uint32_t graphicsQueueFamilyIndex)
    : surface(surface), graphicsQueueFamilyIndex(graphicsQueueFamilyIndex)
{}

SwapchainRenderer::~SwapchainRenderer()
{
    Cleanup();
}

bool SwapchainRenderer::Init(IRenderEngine *renderEngine)
{
    this->renderEngine = dynamic_cast<VulkanRenderEngine*>(renderEngine);
    if (!this->renderEngine)
    {
        return false;
    }

    vulkanSwapchain = new VulkanSwapchain(this->renderEngine, surface, graphicsQueueFamilyIndex);
    
    return true;
}

void SwapchainRenderer::Cleanup()
{
    CleanupSwapchain();
}

bool SwapchainRenderer::BeginRenderCommands()
{
   
    auto res = vkAcquireNextImageKHR(renderEngine->GetDevice(), vulkanSwapchain->GetSwapchain(), UINT64_MAX, vulkanSwapchain->GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);
    if (res == VK_ERROR_OUT_OF_DATE_KHR) 
    {
        RecreateSwapChain();
        return false;
    } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("vkAcquireNextImageKHR failed!");
    }

    VkCommandBuffer commandBuffer = vulkanSwapchain->GetCommandBuffer(imageIndex);
    VkFramebuffer frameBuffer = vulkanSwapchain->GetFrameBuffer(imageIndex);
    VkFence inFlightFence = vulkanSwapchain->GetInFlightFence(imageIndex);
    VkSemaphore renderFinishedSemaphore = vulkanSwapchain->GetRenderFinishedSemaphore(imageIndex);

    vkWaitForFences(renderEngine->GetDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(renderEngine->GetDevice(), 1, &inFlightFence);

    
    // Réinitialise le command buffer
    if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to reset command buffer!");
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin command buffer!");
    }
   
    // Débute le render pass
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderEngine->GetDefaultRenderPass();
    renderPassInfo.framebuffer = vulkanSwapchain->GetFrameBuffers()[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {vulkanSwapchain->GetExtent().width, vulkanSwapchain->GetExtent().height};

    // Couleur de fond (noir) et valeur de profondeur initiale (1.0)
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = clearColor.color; // Couleur de fond
    clearValues[1].depthStencil = {1.0f, 0}; // Valeur de profondeur initiale (1.0 = loin)

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    return true;
}

void SwapchainRenderer::EndRenderCommandsAndPresent(VkQueue presentQueue, VkQueue graphicsQueue)
{

    VkCommandBuffer commandBuffer = vulkanSwapchain->GetCommandBuffer(imageIndex);
    VkFramebuffer frameBuffer = vulkanSwapchain->GetFrameBuffer(imageIndex);
    VkFence inFlightFence = vulkanSwapchain->GetInFlightFence(imageIndex);
    VkSemaphore renderFinishedSemaphore = vulkanSwapchain->GetRenderFinishedSemaphore(imageIndex);

    // Termine le render pass
    vkCmdEndRenderPass(commandBuffer);

    // Termine l'enregistrement du command buffer
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to end command buffer!");
    }


    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {vulkanSwapchain->GetImageAvailableSemaphore()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    VkSwapchainKHR swapChains[] = {vulkanSwapchain->GetSwapchain()};
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    VkResult res = vkQueuePresentKHR(presentQueue, &presentInfo);
    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) 
    {
        RecreateSwapChain();
    } 
    else if (res != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to present swap chain image!");
    }
}

void SwapchainRenderer::SetRenderPass(VkRenderPass renderPass)
{
    vulkanSwapchain->SetRenderPass(renderPass);
}

void SwapchainRenderer::RecreateSwapChain()
{
    vulkanSwapchain->Recreate();
}

void SwapchainRenderer::CleanupSwapchain()
{
    delete vulkanSwapchain;
}