#include "VulkanCamera.h"
#include "IRenderable.h"
#include "ICamera.h"
#include "IRenderTarget.h"
#include <array>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>             // Base de GLM (vecteurs, matrices)
#include <glm/gtc/quaternion.hpp>  // Fonctions de quaternion
#include <glm/gtx/quaternion.hpp>  // Conversion quaternion <-> matrice
#include <glm/gtc/matrix_transform.hpp>

#include "EngineCore.hpp"

#include "VulkanDescriptorPoolBuilder.h"
#include "VulkanDescriptorUtils.h"

#include "VulkanDeviceManager.h"
#include "VulkanPipelineManager.h"

void VulkanCamera::Init(class IRenderTarget* renderTarget)
{

    // Init RenderPass
    CreateDescriptorPool();
    CreateDescriptors();


    if (renderTarget)
        SetRenderTarget(renderTarget);

    viewBuffer.Create(sizeof(ViewProj));

    SetCameraTransform(Transform());
    UpdateProjectionMatrix();
}

bool VulkanCamera::Render(IRenderable* renderable, const VkCommandBuffer& commandBuffer)
{

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(GetRenderTarget()->GetWidth());
    viewport.height = static_cast<float>(GetRenderTarget()->GetHeight());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{}; 
    scissor.offset = {0, 0};
    scissor.extent = {GetRenderTarget()->GetWidth(), GetRenderTarget()->GetHeight()};

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    SetAspectRatio(static_cast<float>(GetRenderTarget()->GetWidth()) / static_cast<float>(GetRenderTarget()->GetHeight()));

    UpdateViewMatrix();

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetVulkanPipelineManager().GetPipelineLayout(), 0, 1, GetDescriptorSet(), 0, nullptr);

    return true;
}

void VulkanCamera::Cleanup()
{
    viewBuffer.Cleanup();

    // Détruire le descriptor pool
    if (cameraDescriptorPool != VK_NULL_HANDLE) 
    {
        vkDestroyDescriptorPool(GetVulkanDeviceManager().GetDeviceChecked(), cameraDescriptorPool, nullptr);
        cameraDescriptorPool = VK_NULL_HANDLE;
    }
}

void VulkanCamera::SetRenderTarget(IRenderTarget *renderTarget)
{
    this->renderTarget = renderTarget;
}

void VulkanCamera::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void VulkanCamera::SetFOV(float fov)
{
    this->fov = fov;
    UpdateProjectionMatrix();
}

void VulkanCamera::SetNearPlan(float nearPlan)
{
    this->nearPlan = nearPlan;
    UpdateProjectionMatrix();
}

void VulkanCamera::SetFarPlan(float farPlan)
{
    this->farPlan = farPlan;
    UpdateProjectionMatrix();
}

void VulkanCamera::SetCameraTransform(Transform transform)
{
    auto r = transform.rotation.ToEuler(ERS_ZYX);
    transform.rotation = Rotator::FromEuler(-r.x, -r.y, -r.z, ERS_XYZ);

    union Mat
    {
        Matrix4<float> mat{0};
        glm::mat4 glmMat;
    };

    Mat mat;
    mat.mat = transform.rotation.GetRotationMatrix();

    glm::mat4 rotationMatrix = mat.glmMat;
    glm::vec3 forward = glm::vec4(glm::vec3( 1, 0, 0), 1.0f) * rotationMatrix; // X+ avant
    glm::vec3 right   = glm::vec4(glm::vec3( 0, 1, 0), 1.0f) * rotationMatrix; // Y+ droite
    glm::vec3 up      = glm::vec4(glm::vec3( 0, 0, 1), 1.0f) * rotationMatrix; // Z+ haut
    glm::vec3 pos = {transform.position.x, transform.position.y, transform.position.z};

    mat.glmMat = glm::lookAt(pos, pos + forward, -up);

    viewData.view = mat.mat;

    UpdateViewMatrix();
}

IRenderTarget *VulkanCamera::GetRenderTarget() const { return renderTarget; }

void VulkanCamera::CreateDescriptorPool()
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 10; // Gérer plusieurs materials

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 10;

    if (vkCreateDescriptorPool(GetVulkanDeviceManager().GetDeviceChecked(), &poolInfo, nullptr, &cameraDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Échec de la création du descriptor pool !");
    }
}

void VulkanCamera::CreateDescriptors()
{
    cameraDescriptorPool = VulkanDescriptorPoolBuilder().AddUniformBuffer(1).SetMaxSets(1).Build();
    cameraDescriptorSet = VulkanDescriptorUtils::AllocateSet(cameraDescriptorPool, GetVulkanPipelineManager().GetCameraDescriptorSetLayout());
}

void VulkanCamera::UpdateViewMatrix()
{
    UpdateViewDataBuffer();
}

void VulkanCamera::UpdateProjectionMatrix()
{
    union Mat
    {
        Matrix4<float> mat{0};
        glm::mat4 glmMat;
    };

    Mat mat;
    mat.glmMat = glm::perspective(glm::radians(fov), aspectRatio, nearPlan, farPlan);

    viewData.proj = mat.mat;
    UpdateViewDataBuffer();
}

void VulkanCamera::UpdateViewDataBuffer()
{
    viewBuffer.Update(&viewData);
            
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = GetViewDataBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(ViewProj);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = *GetDescriptorSet();
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), 1, &descriptorWrite, 0, nullptr);
}

#undef GLM_ENABLE_EXPERIMENTAL