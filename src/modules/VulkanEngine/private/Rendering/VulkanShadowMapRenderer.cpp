#include "VulkanShadowMapRenderer.h"

#include <stdexcept>

#include "Logger.h"
#include "IVulkanMesh.h"
#include "VulkanCamera.h"
#include "VulkanShadowMapRenderTarget.h"
#include "VulkanCommandPoolBuilder.h"
#include "VulkanCommandUtils.h"
#include "VulkanDeviceManager.h"

#include "VulkanShadowMapCamera.h"
#include "IVulkanShadowMap.h"

VulkanShadowMapRenderer &GetVulkanShadowMapRenderer()
{
    return VulkanShadowMapRenderer::GetInstance();
}
void VulkanShadowMapRenderer::Init(VkQueue graphicsQueue, uint32_t queueFamilyIndex)
{
    if (graphicsQueue == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Can't init shadow map renderer, graphics queue is null");
        throw std::runtime_error("Can't init shadow map renderer, graphics queue is null");
    }

    commandPool = VulkanCommandPoolBuilder().AllowResetCommandBuffer()
        .SetQueueFamilyIndex(queueFamilyIndex).Build();

    this->graphicsQueue = graphicsQueue;

    commandBuffer = VulkanCommandUtils::AllocateCommandBuffer(commandPool);
}

void VulkanShadowMapRenderer::Shutdown() noexcept
{
    auto device = GetVulkanDeviceManager().GetDeviceChecked();

    VulkanCommandUtils::FreeCommandBuffer(commandPool, commandBuffer);
    VulkanCommandUtils::DestroyCommandPool(commandPool);
}

void VulkanShadowMapRenderer::Render(VulkanShadowMapRenderTarget* shadowMap, VulkanCamera *lightViewCamera, IVulkanMesh **meshes, uint32_t meshNumber)
{
    //TODO: Assert function arguments
    
    if (GetGraphicsQueue() == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Can't render shadow map, graphics queue is null");
        throw std::runtime_error("Can't render shadow map, graphics queue is null");
    }

    if (GetRenderPass() == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Can't render shadow map, render pass is null");
        throw std::runtime_error("Can't render shadow map, render pass is null");
    }

    VulkanCommandUtils::ResetCommandBuffer(commandBuffer);
    VulkanCommandUtils::BeginCommandBuffer(commandBuffer);

    // Clear value pour la profondeur
    VkClearValue clearValue;
    clearValue.depthStencil = {1.0f, 0}; // 1.0 = loin

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass; // Utilisez le bon render pass
    renderPassInfo.framebuffer = shadowMap->GetFramebuffer();
    renderPassInfo.renderArea.extent = {shadowMap->GetWidth(), shadowMap->GetHeight()};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    lightViewCamera->Render(nullptr, commandBuffer);

    vkCmdSetDepthBias(commandBuffer, 0.0f, 0.0f, 0.0f);

    for (uint32_t i = 0; i < meshNumber; i++)
    {
        if (meshes[i])
            meshes[i]->DrawVulkanMesh(commandBuffer, ERenderPassType::RENDER_PASS_TYPE_SHADOWMAP);
    }

    vkCmdEndRenderPass(commandBuffer);

    VulkanCommandUtils::EndCommandBuffer(commandBuffer);
    VulkanCommandUtils::SubmitCommandBuffer(GetGraphicsQueue(), commandBuffer);

    if(auto result = vkQueueWaitIdle(GetGraphicsQueue()); result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to wait idle queue");
    }
}

void VulkanShadowMapRenderer::Render(VulkanShadowMapCamera *shadowMapCamera, IVulkanMesh **meshes, uint32_t meshNumber)
{
    if (GetGraphicsQueue() == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Can't render shadow map, graphics queue is null");
        throw std::runtime_error("Can't render shadow map, graphics queue is null");
    }

    if (GetRenderPass() == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Can't render shadow map, render pass is null");
        throw std::runtime_error("Can't render shadow map, render pass is null");
    }


    shadowMapCamera->GetShadowMapTarget()->BeginRendering(commandBuffer);

    // Clear value pour la profondeur
    VkClearValue clearValue;
    clearValue.depthStencil = {1.0f, 0}; // 1.0 = loin

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = shadowMapCamera->GetShadowMapTarget()->GetFramebuffer();
    renderPassInfo.renderArea.extent = {
        shadowMapCamera->GetShadowMapTarget()->GetResolution(), 
        shadowMapCamera->GetShadowMapTarget()->GetResolution()
    };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    shadowMapCamera->Bind(commandBuffer);

    vkCmdSetDepthBias(commandBuffer, 0.0f, 0.0f, 0.0f);

    for (uint32_t i = 0; i < meshNumber; i++)
    {
        if (meshes[i])
            meshes[i]->DrawVulkanMesh(commandBuffer, ERenderPassType::RENDER_PASS_TYPE_SHADOWMAP);
    }

    vkCmdEndRenderPass(commandBuffer);

    shadowMapCamera->GetShadowMapTarget()->EndRendering(GetGraphicsQueue(), commandBuffer);

    
}
