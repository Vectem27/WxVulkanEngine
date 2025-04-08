#include "VulkanProjectorLight.h"
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanCamera.h"
#include "VulkanRenderEngine.h"
#include "VulkanRenderTarget.h"

VulkanProjectorLight::~VulkanProjectorLight()
{
    delete camera;
    delete renderTarget;
}

void VulkanProjectorLight::InitVulkanProjectorLight(VulkanRenderEngine *renderEngine)
{
    renderTarget = new VulkanRenderTarget(renderEngine, 1024, 1024, renderEngine->GetDepthStencilImageFormat());
    renderTarget->CreateFramebuffer(renderEngine->GetShadowMapRenderPass());
    camera = new VulkanCamera();
    camera->Init(renderEngine, renderTarget);
    camera->SetFarPlan(50.0f);
    camera->SetNearPlan(0.1f);
    camera->SetFOV(90.0f);
    SetShadowMap(renderTarget->GetImageView(), renderEngine->GetPipelineManager()->GetShadowMapSampler());
}

ProjectorLightData VulkanProjectorLight::GetProjectorLightData() const
{
    return data;
}

void VulkanProjectorLight::SetTransform(Transform transform)
{
    if (!camera)
        throw std::runtime_error("VulkanProjectorLight : camera is not initialized !");
    camera->SetCameraTransform(transform);

    data.viewProj = camera->GetViewData().view * camera->GetViewData().proj;

}
