#include "VulkanSpotlightLight.h"
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanCamera.h"
#include "VulkanRenderEngine.h"
#include "VulkanRenderTarget.h"
#include "VulkanRenderPassManager.h"

const VulkanLightType VulkanSpotlightLight::lightType{"SPOTLIGHT"};

VulkanSpotlightLight::~VulkanSpotlightLight()
{
    delete camera;
    delete renderTarget;
}

void VulkanSpotlightLight::InitVulkanSpotlightLight(VulkanRenderEngine *renderEngine)
{
    renderTarget = new VulkanRenderTarget(renderEngine, 1024, 1024, VulkanRenderPassManager::GetInstance()->GetShadowMapFormat());
    renderTarget->CreateFramebuffer(
        VulkanRenderPassManager::GetInstance()->GetShadowPass()
    );

    camera = new VulkanCamera();
    camera->Init(renderEngine, renderTarget);
    camera->SetNearPlan(0.1f);
    UpdateCameraProperties();
    SetShadowMap(renderTarget->GetImageView());
}

ProjectorLightData VulkanSpotlightLight::GetSpotlightLightData() const
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
}

void VulkanSpotlightLight::UpdateCameraProperties()
{
    camera->SetFOV(ToDegree(data.angle + data.softAngle));
    camera->SetFarPlan(data.length);
}
