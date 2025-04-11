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

#include "VulkanShadowMapRenderer.h"
#include "VulkanRenderTargetRenderer.h"

//Temp
#include "VulkanSpotlightLightPipeline.h"
#include "VulkanPostprocessPipeline.h"

#include "VulkanCommandUtils.h"

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


    VkCommandBuffer commandBuffer = swapchain->GetCommandBuffer();
    VkFramebuffer frameBuffer = swapchain->GetGeometryFrameBuffer();
    VkFramebuffer lightingFrameBuffer = swapchain->GetLightingFrameBuffer();
    VkFramebuffer postprocessFrameBuffer = swapchain->GetPostprocessFrameBuffer();


    std::vector<IVulkanMesh*> meshes;

    Array<IRenderable*> scene;
    if (renderObject)
        renderObject->CollectAllRenderChilds(scene, ERenderPassType::RENDER_PASS_TYPE_DEFAULT);

    lightManager->ClearLights();
    for (auto& object : scene)
    {
        auto light = dynamic_cast<VulkanSpotlightLight*>(object);
        if(light)
        {
            lightManager->AddLight(light);
            RenderToShadowMap(light->renderTarget, renderObject, light->camera, graphicsQueue);
        }

        auto mesh = reinterpret_cast<IVulkanMesh*>(object->GetRenderMesh());
        if(mesh)
            meshes.push_back(mesh);
    }

    static bool doOnce{true};
    if (doOnce)
    {
        lightManager->Update();
        doOnce = false;
    }

    GetVulkanRenderTargetRenderer().Render(swapchain, camera, meshes.data(), meshes.size(), lightManager);

    

    

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


    Array<IRenderable*> scene;
    if (renderObject)
        renderObject->CollectAllRenderChilds(scene, ERenderPassType::RENDER_PASS_TYPE_SHADOWMAP);

    std::vector<IVulkanMesh*> meshes;
    for (const auto& object : scene)
    {
        auto mesh = reinterpret_cast<IVulkanMesh*>(object->GetRenderMesh());
        if (mesh)
            meshes.push_back(mesh);
    }

    GetVulkanShadowMapRenderer().Render(renderTarget, light, meshes.data(), meshes.size());

    return true;
}
