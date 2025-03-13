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
    
    CreateCommandPool();
    CreateCommandBuffers();
    CreateSync();

    return true;
}

void SwapchainRenderer::Cleanup()
{
    CleanupSwapchain();

    
    // Détruire les sémaphores et la fence
    if (imageAvailableSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(renderEngine->GetDevice(), imageAvailableSemaphore, nullptr);
        imageAvailableSemaphore = VK_NULL_HANDLE;
    }
    if (renderFinishedSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(renderEngine->GetDevice(), renderFinishedSemaphore, nullptr);
        renderFinishedSemaphore = VK_NULL_HANDLE;
    }
    if (inFlightFence != VK_NULL_HANDLE) {
        vkDestroyFence(renderEngine->GetDevice(), inFlightFence, nullptr);
        inFlightFence = VK_NULL_HANDLE;
    }

    // Détruire le command pool
    if (commandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(renderEngine->GetDevice(), commandPool, nullptr);
        commandPool = VK_NULL_HANDLE;
    }
}

bool SwapchainRenderer::BeginRenderCommands()
{
   
    auto res = vkAcquireNextImageKHR(renderEngine->GetDevice(), vulkanSwapchain->GetSwapchain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    if (res == VK_ERROR_OUT_OF_DATE_KHR) 
    {
        RecreateSwapChain();
        return false;
    } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("vkAcquireNextImageKHR failed!");
    }

    vkWaitForFences(renderEngine->GetDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(renderEngine->GetDevice(), 1, &inFlightFence);

    
    // Réinitialise le command buffer
    if (vkResetCommandBuffer(commandBuffers[imageIndex], 0) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to reset command buffer!");
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
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

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    return true;
}

void SwapchainRenderer::EndRenderCommandsAndPresent(VkQueue presentQueue, VkQueue graphicsQueue)
{

    // Termine le render pass
    vkCmdEndRenderPass(commandBuffers[imageIndex]);

    // Termine l'enregistrement du command buffer
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
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
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

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

void SwapchainRenderer::CreateCommandPool()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    if (vkCreateCommandPool(renderEngine->GetDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

void SwapchainRenderer::CreateCommandBuffers()
{
    commandBuffers.resize(vulkanSwapchain->GetImageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(renderEngine->GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void SwapchainRenderer::CreateSync()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(renderEngine->GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(renderEngine->GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // La fence est initialement signalée

    if (vkCreateFence(renderEngine->GetDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create fence!");
    }
}

void SwapchainRenderer::RecreateSwapChain()
{
    vulkanSwapchain->Recreate();

    // Détruire les anciens command buffers avant de les recréer
    if (!commandBuffers.empty()) 
    {
        vkFreeCommandBuffers(renderEngine->GetDevice(), commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }
    CreateCommandBuffers();
}

void SwapchainRenderer::CleanupSwapchain()
{
    delete vulkanSwapchain;
}