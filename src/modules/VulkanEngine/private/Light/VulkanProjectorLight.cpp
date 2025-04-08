#include "VulkanProjectorLight.h"
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanCamera.h"
#include "VulkanRenderEngine.h"
#include "VulkanRenderTarget.h"

VulkanSpotlightLight::~VulkanSpotlightLight()
{
    delete camera;
    delete renderTarget;
}

void VulkanSpotlightLight::InitVulkanProjectorLight(VulkanRenderEngine *renderEngine)
{
    renderTarget = new VulkanRenderTarget(renderEngine, 1024, 1024, renderEngine->GetDepthStencilImageFormat());
    renderTarget->CreateFramebuffer(renderEngine->GetShadowMapRenderPass());
    camera = new VulkanCamera();
    camera->Init(renderEngine, renderTarget);
    camera->SetFarPlan(100.0f);
    camera->SetNearPlan(0.1f);
    camera->SetFOV(30.0f);
    SetShadowMap(renderTarget->GetImageView(), renderEngine->GetPipelineManager()->GetShadowMapSampler());
}

ProjectorLightData VulkanSpotlightLight::GetProjectorLightData() const
{
    return data;
}

void VulkanSpotlightLight::SetTransform(Transform transform)
{
    if (!camera)
        throw std::runtime_error("VulkanProjectorLight : camera is not initialized !");
    camera->SetCameraTransform(transform);

    data.viewProj = camera->GetViewData().view * camera->GetViewData().proj;
    data.position = transform.position;
    data.direction = transform.rotation.Rotate({1.0f, 0.0f, 0.0f});
    data.length = 100;
    data.angle = ToRadians(30.0f);
}
