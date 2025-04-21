#include "VulkanRenderTargetRenderer.h"


#include <stdexcept>

#include "Logger.h"
#include "IVulkanMesh.h"
#include "VulkanCamera.h"
#include "IVulkanRenderTarget.h"
#include "VulkanCommandPoolBuilder.h"
#include "VulkanCommandUtils.h"
#include "LightManagers/IVulkanLightManager.h"
#include "VulkanDeviceManager.h"

//TODO: Remove
#include "VulkanSpotlightLightPipeline.h"
#include "VulkanPostprocessPipeline.h"
#include "VulkanPipelineManager.h"

VulkanRenderTargetRenderer &GetVulkanRenderTargetRenderer()
{
    return VulkanRenderTargetRenderer::GetInstance();
}

void VulkanRenderTargetRenderer::Init(VkQueue graphicsQueue, uint32_t queueFamilyIndex)
{
    if (graphicsQueue == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Can't init shadow map renderer, graphics queue is null");
        throw std::runtime_error("Can't init shadow map renderer, graphics queue is null");
    }

    this->graphicsQueue = graphicsQueue;

    commandPool = VulkanCommandPoolBuilder().AllowResetCommandBuffer()
        .SetQueueFamilyIndex(queueFamilyIndex).Build();

    commandBuffer = VulkanCommandUtils::AllocateCommandBuffer(commandPool);

    postprocessPipeline = new VulkanPostprocessPipeline();
    postprocessPipeline->InitPipeline(GetVulkanDeviceManager().GetDeviceChecked(), &VulkanPipelineManager::GetInstance(),
        "shaders/fullScreen.vert", "shaders/postprocess.frag");
}

void VulkanRenderTargetRenderer::Shutdown() noexcept
{
    auto device = GetVulkanDeviceManager().GetDeviceChecked();

    VulkanCommandUtils::FreeCommandBuffer(commandPool, commandBuffer);
    VulkanCommandUtils::DestroyCommandPool(commandPool);
}

void VulkanRenderTargetRenderer::Render(IVulkanRenderTarget *renderTarget, VulkanCamera *camera, IVulkanMesh **meshes, uint32_t meshNumber, const IVulkanLightManager& lightManager, std::function<void(VkCommandBuffer)> renderCallback) const
{
    //TODO: Assert function arguments

    renderTarget->BeginRendering(commandBuffer);

    {
        // Débute le render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = GetGeometryRenderPass();
        renderPassInfo.framebuffer = renderTarget->GetGeometryFrameBuffer();
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = {renderTarget->GetWidth(), renderTarget->GetHeight()};

        // Couleur de fond (noir) et valeur de profondeur initiale (1.0)
        std::array<VkClearValue, 4> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};  // Base color
        clearValues[1].depthStencil = {1.0f, 0}; // Depth stencil (1.0 = far)
        clearValues[2].color = {0.0f, 0.0f, 0.0f, 1.0f};  // world normal
        clearValues[3].color = {0.0f, 0.0f, 0.0f, 1.0f};  // world position

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();


        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Render
        camera->Render(commandBuffer);

        if (renderCallback)
            renderCallback(commandBuffer);

        for (size_t i = 0; i < meshNumber; i++)
        {
            meshes[i]->Draw(commandBuffer);
        }
        
        // Termine le render pass
        vkCmdEndRenderPass(commandBuffer);
    }

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)renderTarget->GetWidth();
    viewport.height = (float)renderTarget->GetHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {renderTarget->GetWidth(), renderTarget->GetHeight()};

    {
        // Débute le render pass
        VkRenderPassBeginInfo lightingRenderPassInfo{};
        lightingRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        lightingRenderPassInfo.renderPass = GetLightingRenderPass();
        lightingRenderPassInfo.framebuffer = renderTarget->GetLightingFrameBuffer();
        lightingRenderPassInfo.renderArea.offset = {0, 0};
        lightingRenderPassInfo.renderArea.extent = {renderTarget->GetWidth(), renderTarget->GetHeight()};

        // Couleur de fond (noir) et valeur de profondeur initiale (1.0)
        std::array<VkClearValue, 1> ligtingClearValues{};
        ligtingClearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};

        lightingRenderPassInfo.clearValueCount = ligtingClearValues.size();
        lightingRenderPassInfo.pClearValues = ligtingClearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &lightingRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        renderTarget->StartLighting(commandBuffer);

        lightManager.Bind(commandBuffer);
    }

    

    {
        // Débute le render pass
        VkRenderPassBeginInfo postprocessRenderPassInfo{};
        postprocessRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        postprocessRenderPassInfo.renderPass = GetPostprocessRenderPass();
        postprocessRenderPassInfo.framebuffer = renderTarget->GetPostprocessFrameBuffer();
        postprocessRenderPassInfo.renderArea.offset = {0, 0};
        postprocessRenderPassInfo.renderArea.extent = {renderTarget->GetWidth(), renderTarget->GetHeight()};
        // Couleur de fond (noir) et valeur de profondeur initiale (1.0)
        std::array<VkClearValue, 1> postprocessClearValues{};
        postprocessClearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        
        postprocessRenderPassInfo.clearValueCount = postprocessClearValues.size();
        postprocessRenderPassInfo.pClearValues = postprocessClearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &postprocessRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        postprocessPipeline->Bind(commandBuffer);

        renderTarget->StartPostprocessing(commandBuffer);
        
        vkCmdDraw(commandBuffer, 4, 1, 0, 0);
    }

    vkCmdEndRenderPass(commandBuffer);

    renderTarget->EndRendering(graphicsQueue, commandBuffer);
}
