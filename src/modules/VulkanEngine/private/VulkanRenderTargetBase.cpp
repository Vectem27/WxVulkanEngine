#include "VulkanRenderTargetBase.h"
#include "VulkanRenderer.h"

#include <array>
#include <vulkan/vulkan.h>

bool VulkanRenderTargetBase::Init(IRenderEngine *renderEngine)
{
    this->renderEngine;
    return true;
}

bool VulkanRenderTargetBase::BeginRenderCommands()
{
    
    vkWaitForFences(renderEngine->GetDevice(), 1, &GetCurrentRenderFence(), VK_TRUE, UINT64_MAX);
    vkResetFences(renderEngine->GetDevice(), 1, &GetCurrentRenderFence());

    
    // Réinitialise le command buffer
    if (vkResetCommandBuffer(GetCurrentCommandBuffer(), 0) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to reset command buffer!");
    }
    
    // Réinitialise le command buffer
    if (vkResetCommandBuffer(GetCurrentCommandBuffer(), 0) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to reset command buffer!");
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin command buffer!");
    }
   
    // Débute le render pass
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderEngine->GetDefaultRenderPass();
    renderPassInfo.framebuffer = GetCurrentFrameBuffer();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {width, height};

    // Couleur de fond (noir) et valeur de profondeur initiale (1.0)
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = clearColor; // Couleur de fond
    clearValues[1].depthStencil = {1.0f, 0}; // Valeur de profondeur initiale (1.0 = loin)

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();


    vkCmdBeginRenderPass(GetCurrentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    return true;
}

void VulkanRenderTargetBase::EndRenderCommands()
{
    // Termine le render pass
    vkCmdEndRenderPass(GetCurrentCommandBuffer());

    // Termine l'enregistrement du command buffer
    if (vkEndCommandBuffer(GetCurrentCommandBuffer()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to end command buffer!");
    }
}

void VulkanRenderTargetBase::Present()
{

}
