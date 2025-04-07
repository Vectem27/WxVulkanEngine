#include "VulkanRenderer.h"

#include "VulkanRenderEngine.h"
#include "IRenderable.h"
#include "VulkanCamera.h"
#include "VulkanLight.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderTarget.h"
#include <array>

VulkanRenderer::VulkanRenderer(VulkanRenderEngine *renderEngine)
    : renderEngine(renderEngine)
{
}

bool VulkanRenderer::RenderToSwapchain(VulkanSwapchain *swapchain, IRenderable *renderObject, VulkanCamera *camera, VkQueue graphicsQueue, VkQueue presentQueue)
{
    uint32_t imageIndex;
    VkSemaphore imageAvailableSemaphore = swapchain->GetImageAvailableSemaphore();
    auto res = vkAcquireNextImageKHR(renderEngine->GetDevice(), swapchain->GetSwapchain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    if (res == VK_ERROR_OUT_OF_DATE_KHR) 
    {
        swapchain->Recreate();
        return false;
    } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) 
    {
        throw std::runtime_error("vkAcquireNextImageKHR failed!");
    }

    VkCommandBuffer commandBuffer = swapchain->GetCommandBuffer(imageIndex);
    VkFramebuffer frameBuffer = swapchain->GetFrameBuffer(imageIndex);
    VkFence inFlightFence = swapchain->GetInFlightFence();
    VkSemaphore renderFinishedSemaphore = swapchain->GetRenderFinishedSemaphore();


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
    renderPassInfo.framebuffer = frameBuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {swapchain->GetExtent().width, swapchain->GetExtent().height};

    // Couleur de fond (noir) et valeur de profondeur initiale (1.0)
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f}; // Couleur de fond
    clearValues[1].depthStencil = {1.0f, 0}; // Valeur de profondeur initiale (1.0 = loin)

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Render
    camera->Render(renderObject, commandBuffer);
    if (renderObject)
        renderObject->Draw(commandBuffer, ERenderPassType::RENDER_PASS_TYPE_DEFAULT);

    // Termine le render pass
    vkCmdEndRenderPass(commandBuffer);

    // Termine l'enregistrement du command buffer
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to end command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
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
    VkSwapchainKHR swapChains[] = {swapchain->GetSwapchain()};
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    res = vkQueuePresentKHR(presentQueue, &presentInfo);
    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) 
    {
        swapchain->Recreate();
    } 
    else if (res != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    return true;
}

bool VulkanRenderer::RenderToShadowMap(VulkanRenderTarget *renderTarget, IRenderable *renderObject, VulkanLight *light, VkQueue graphicsQueue)
{
    const auto& commandBuffer = renderTarget->GetCommandBuffer();

    if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) 
        throw std::runtime_error("failed to reset command buffer!");

    // Begin command buffer
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    // Clear value pour la profondeur
    VkClearValue clearValue;
    clearValue.depthStencil = {1.0f, 0}; // 1.0 = loin

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderEngine->GetShadowMapRenderPass(); // Utilisez le bon render pass
    renderPassInfo.framebuffer = renderTarget->GetFramebuffer();
    renderPassInfo.renderArea.extent = {renderTarget->GetWidth(), renderTarget->GetHeight()};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    // Render
    light->Render(renderObject, commandBuffer);
    if (renderObject)
        renderObject->Draw(commandBuffer, ERenderPassType::RENDER_PASS_TYPE_SHADOWMAP);
    
    vkCmdEndRenderPass(commandBuffer);
    vkEndCommandBuffer(commandBuffer);

    // Submit
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    return true;
}
