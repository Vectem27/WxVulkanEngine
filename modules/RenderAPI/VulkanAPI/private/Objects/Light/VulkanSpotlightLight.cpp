#include "VulkanSpotlightLight.h"
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanRenderPassManager.h"

#include "VulkanShadowMapCamera.h"
#include "VulkanShadowMap.h"

VulkanSpotlightLight::VulkanSpotlightLight()
{
    shadowMap = new VulkanShadowMap(1024);

    shadowMapCamera = new VulkanShadowMapCamera();
    shadowMapCamera->SetShadowMap(shadowMap);
    shadowMapCamera->SetNearPlan(0.1f);
    UpdateCameraProperties();
    SetShadowMap(shadowMap->GetImageView());
}

VulkanSpotlightLight::~VulkanSpotlightLight()
{
    delete shadowMapCamera;
    delete shadowMap;
}

Array<VulkanShadowMapCamera *> VulkanSpotlightLight::GetShadowMapCameras() const
{
    return Array<VulkanShadowMapCamera *>({shadowMapCamera});
}

void VulkanSpotlightLight::SetTransform(Transform transform)
{
    if (!shadowMapCamera)
        throw std::runtime_error("VulkanProjectorLight : camera is not initialized !");
    shadowMapCamera->SetTransform(transform);

    viewProj = shadowMapCamera->GetViewMatrix() * shadowMapCamera->GetProjMatrix();
    position = transform.position;
    direction = transform.rotation.Rotate({1.0f, 0.0f, 0.0f});
}

void VulkanSpotlightLight::UpdateCameraProperties()
{
    shadowMapCamera->SetFOV((GetLightAngle() + GetLightSoftAngle()));
    shadowMapCamera->SetFarPlan(GetLightLength());
}
