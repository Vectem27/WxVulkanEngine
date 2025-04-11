#include "VulkanRenderer.h"

#include "VulkanRenderEngine.h"
#include "IRenderable.h"
#include "VulkanCamera.h"
#include "VulkanSwapchain.h"
#include "VulkanShadowMapRenderTarget.h"
#include <array>
#include "IVulkanMesh.h"
#include "VulkanGlobalLightManager.h"
#include "VulkanSpotlightLight.h"
#include "VulkanRenderPassManager.h"

//Temp
#include "VulkanSpotlightLightPipeline.h"
#include "VulkanPostprocessPipeline.h"

VulkanRenderer::VulkanRenderer(VulkanRenderEngine *renderEngine)
    : renderEngine(renderEngine)
{
    spotlightLightPipeline = new VulkanSpotlightLightPipeline();
    spotlightLightPipeline->InitPipeline(GetVulkanDeviceManager().GetDeviceChecked(), renderEngine->GetPipelineManager(),
        "shaders/fullScreen.vert", "shaders/lighting.frag");

    postprocessPipeline = new VulkanPostprocessPipeline();
    postprocessPipeline->InitPipeline(GetVulkanDeviceManager().GetDeviceChecked(), renderEngine->GetPipelineManager(),
        "shaders/fullScreen.vert", "shaders/postprocess.frag");
}

bool VulkanRenderer::RenderToSwapchain(VulkanSwapchain *swapchain, IRenderable *renderObject, VulkanCamera *camera, VulkanGlobalLightManager* lightManager, VkQueue graphicsQueue, VkQueue presentQueue)
{
    if (!swapchain)
        throw std::invalid_argument("Swapchain is invalid");

    if (!renderObject)
        throw std::invalid_argument("Render object is invalid");

    if (!camera)
        throw std::invalid_argument("Camera is invalid");


    uint32_t imageIndex;
    VkSemaphore imageAvailableSemaphore = swapchain->GetImageAvailableSemaphore();
    swapchain->StartRendering();

    VkCommandBuffer commandBuffer = swapchain->GetCommandBuffer();
    VkFramebuffer frameBuffer = swapchain->GetFrameBuffer();
    VkFramebuffer lightingFrameBuffer = swapchain->GetLightingFrameBuffer();
    VkFramebuffer postprocessFrameBuffer = swapchain->GetPostprocessFrameBuffer();
    VkFence inFlightFence = swapchain->GetInFlightFence();
    VkSemaphore renderFinishedSemaphore = swapchain->GetRenderFinishedSemaphore();

    // Setup lights
    Array<IRenderable*> scene;
    if (renderObject)
        renderObject->CollectAllRenderChilds(scene, ERenderPassType::RENDER_PASS_TYPE_DEFAULT);

    lightManager->ClearLights();
    for (const auto& object : scene)
    {
        auto light = dynamic_cast<VulkanSpotlightLight*>(object);
        if(light)
        {
            lightManager->AddLight(light);
            RenderToShadowMap(light->renderTarget, renderObject, light->camera, graphicsQueue);

        }
    }

    vkWaitForFences(GetVulkanDeviceManager().GetDeviceChecked(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(GetVulkanDeviceManager().GetDeviceChecked(), 1, &inFlightFence);


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
    renderPassInfo.renderPass = VulkanRenderPassManager::GetInstance()->GetGeometryPass();
    renderPassInfo.framebuffer = frameBuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {swapchain->GetExtent().width, swapchain->GetExtent().height};

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
    camera->Render(renderObject, commandBuffer);

    
    static bool doOnce{true};
    if (doOnce)
    {
        lightManager->Update();
        doOnce = false;
    }

    IVulkanMesh* mesh;
    for (const auto& object : scene)
    {
        mesh = reinterpret_cast<IVulkanMesh*>(object->GetRenderMesh());
        if (mesh)
        {
            mesh->DrawVulkanMesh(commandBuffer, ERenderPassType::RENDER_PASS_TYPE_DEFAULT);
        }
    }

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


    
    vkWaitForFences(GetVulkanDeviceManager().GetDeviceChecked(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(GetVulkanDeviceManager().GetDeviceChecked(), 1, &inFlightFence);

    if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) 
        throw std::runtime_error("failed to reset command buffer!");


    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        throw std::runtime_error("failed to begin command buffer!");

    // Débute le render pass
    VkRenderPassBeginInfo lightingRenderPassInfo{};
    lightingRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    lightingRenderPassInfo.renderPass = VulkanRenderPassManager::GetInstance()->GetLightingPass();
    lightingRenderPassInfo.framebuffer = lightingFrameBuffer;
    lightingRenderPassInfo.renderArea.offset = {0, 0};
    lightingRenderPassInfo.renderArea.extent = {swapchain->GetExtent().width, swapchain->GetExtent().height};

    // Couleur de fond (noir) et valeur de profondeur initiale (1.0)
    std::array<VkClearValue, 1> ligtingClearValues{};
    ligtingClearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};

    lightingRenderPassInfo.clearValueCount = ligtingClearValues.size();
    lightingRenderPassInfo.pClearValues = ligtingClearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &lightingRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapchain->GetExtent().width;
    viewport.height = (float)swapchain->GetExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain->GetExtent();
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    spotlightLightPipeline->Bind(commandBuffer);

    swapchain->UpdateGBufferDescriptorSet();
    vkCmdBindDescriptorSets(
        commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        renderEngine->GetPipelineManager()->GetLightingPipelineLayout(),
        0, 1, &swapchain->GetGBufferDescriptorSet(), 0, nullptr
    );

    lightManager->Bind(commandBuffer);

    // Termine le render pass
    vkCmdEndRenderPass(commandBuffer);


    // Termine l'enregistrement du command buffer
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to end command buffer!");

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) 
        throw std::runtime_error("failed to submit draw command buffer!");
    


    vkWaitForFences(GetVulkanDeviceManager().GetDeviceChecked(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(GetVulkanDeviceManager().GetDeviceChecked(), 1, &inFlightFence);

    if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) 
        throw std::runtime_error("failed to reset command buffer!");


    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        throw std::runtime_error("failed to begin command buffer!");

    // Débute le render pass
    VkRenderPassBeginInfo postprocessRenderPassInfo{};
    postprocessRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    postprocessRenderPassInfo.renderPass = VulkanRenderPassManager::GetInstance()->GetPostprocessPass();
    postprocessRenderPassInfo.framebuffer = postprocessFrameBuffer;
    postprocessRenderPassInfo.renderArea.offset = {0, 0};
    postprocessRenderPassInfo.renderArea.extent = {swapchain->GetExtent().width, swapchain->GetExtent().height};
    // Couleur de fond (noir) et valeur de profondeur initiale (1.0)
    std::array<VkClearValue, 1> postprocessClearValues{};
    postprocessClearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
    
    postprocessRenderPassInfo.clearValueCount = postprocessClearValues.size();
    postprocessRenderPassInfo.pClearValues = postprocessClearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &postprocessRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    postprocessPipeline->Bind(commandBuffer);

    swapchain->UpdatePostprocessDescriptorSet();
    vkCmdBindDescriptorSets(
        commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        renderEngine->GetPipelineManager()->GetPostprocessPipelineLayout(),
        0, 1, &swapchain->GetPostprocessDescriptorSet(), 0, nullptr
    );

    vkCmdDraw(commandBuffer, 4, 1, 0, 0);

    // Termine le render pass
    vkCmdEndRenderPass(commandBuffer);

    // Termine l'enregistrement du command buffer
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to end command buffer!");

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) 
        throw std::runtime_error("failed to submit draw command buffer!");

    return true;
}

bool VulkanRenderer::RenderToShadowMap(VulkanShadowMapRenderTarget *renderTarget, IRenderable *renderObject, VulkanCamera *light, VkQueue graphicsQueue)
{
    if (!renderTarget)
        throw std::invalid_argument("Light render target is invalid");

    if (!renderObject)
        throw std::invalid_argument("Render object is invalid");

    if (!light)
        throw std::invalid_argument("Light camera is invalid");

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
    renderPassInfo.renderPass = VulkanRenderPassManager::GetInstance()->GetShadowPass(); // Utilisez le bon render pass
    renderPassInfo.framebuffer = renderTarget->GetFramebuffer();
    renderPassInfo.renderArea.extent = {renderTarget->GetWidth(), renderTarget->GetHeight()};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    // Render
    light->Render(renderObject, commandBuffer);

    Array<IRenderable*> scene;
    if (renderObject)
        renderObject->CollectAllRenderChilds(scene, ERenderPassType::RENDER_PASS_TYPE_SHADOWMAP);

    IVulkanMesh* mesh;
    for (const auto& object : scene)
    {
        mesh = reinterpret_cast<IVulkanMesh*>(object->GetRenderMesh());
        if (mesh)
        {
            mesh->DrawVulkanMesh(commandBuffer, ERenderPassType::RENDER_PASS_TYPE_SHADOWMAP);
        }
    }


    
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
