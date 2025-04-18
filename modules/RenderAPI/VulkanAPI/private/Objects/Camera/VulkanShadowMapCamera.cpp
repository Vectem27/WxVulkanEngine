#include "VulkanShadowMapCamera.h"

#include "VulkanDescriptorPoolBuilder.h"
#include "VulkanDescriptorUtils.h"
#include "IVulkanShadowMap.h"
#include "VulkanPipelineManager.h"
#include "VulkanDeviceManager.h"

#include "Matrix.hpp"

#include <glm/glm.hpp>             // Base de GLM (vecteurs, matrices)
#include <glm/gtc/matrix_transform.hpp>

struct ShadowMapCameraBuffer
{
    alignas(16) Matrix4f view;
    alignas(16) Matrix4f proj;
};

VulkanShadowMapCamera::VulkanShadowMapCamera()
{
    descriptorPool = VulkanDescriptorPoolBuilder().SetMaxSets(1).AddUniformBuffer(1).Build();
    descriptorSet = VulkanDescriptorUtils::AllocateSet(descriptorPool, GetVulkanPipelineManager().GetCameraDescriptorSetLayout());

    cameraBuffer.Create(sizeof(ShadowMapCameraBuffer));

    SetTransform(Transform());
    UpdateProjectionMatrix();
}

VulkanShadowMapCamera::~VulkanShadowMapCamera()
{
    cameraBuffer.Cleanup();
    VulkanDescriptorUtils::DestroyDescriptorPool(descriptorPool);
}

void VulkanShadowMapCamera::SetShadowMap(IVulkanShadowMap* shadowMap)
{
    this->shadowMap = shadowMap;
}

void VulkanShadowMapCamera::SetTransform(Transform transform)
{
    this->transform = transform;

    UpdateViewMatrix();
}

void VulkanShadowMapCamera::SetFOV(float fov)
{
    this->fov = fov;
    UpdateProjectionMatrix();
}

void VulkanShadowMapCamera::SetNearPlan(float nearPlan)
{
    this->nearPlan = nearPlan;
    UpdateProjectionMatrix();
}

void VulkanShadowMapCamera::SetFarPlan(float farPlan)
{
    this->farPlan = farPlan;
    UpdateProjectionMatrix();
}

void VulkanShadowMapCamera::Bind(VkCommandBuffer commandBuffer) const
{
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(GetShadowMapTarget()->GetResolution());
    viewport.height = static_cast<float>(GetShadowMapTarget()->GetResolution());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{}; 
    scissor.offset = {0, 0};
    scissor.extent = {GetShadowMapTarget()->GetResolution(), GetShadowMapTarget()->GetResolution()};

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetVulkanPipelineManager().GetPipelineLayout(), 0, 1, &GetCameraDescriptorSet(), 0, nullptr);
}

void VulkanShadowMapCamera::UpdateViewMatrix()
{
    auto r = transform.rotation.ToEuler(ERS_ZYX);
    auto rotation = Rotator::FromEuler(-r.x, -r.y, -r.z, ERS_XYZ);

    union Mat
    {
        Matrix4<float> mat{0};
        glm::mat4 glmMat;
    };

    Mat mat;
    mat.mat = rotation.GetRotationMatrix();

    glm::mat4 rotationMatrix = mat.glmMat;
    glm::vec3 forward = glm::vec4(glm::vec3( 1, 0, 0), 1.0f) * rotationMatrix; // X+ avant
    glm::vec3 right   = glm::vec4(glm::vec3( 0, 1, 0), 1.0f) * rotationMatrix; // Y+ droite
    glm::vec3 up      = glm::vec4(glm::vec3( 0, 0, 1), 1.0f) * rotationMatrix; // Z+ haut
    glm::vec3 pos = {transform.position.x, transform.position.y, transform.position.z};

    mat.glmMat = glm::lookAt(pos, pos + forward, -up);

    viewMatrix = mat.mat;

    UpdateCameraBuffer();
}

void VulkanShadowMapCamera::UpdateProjectionMatrix()
{
    union Mat
    {
        Matrix4<float> mat{0};
        glm::mat4 glmMat;
    };

    Mat mat;
    mat.glmMat = glm::perspective(fov, aspectRatio, nearPlan, farPlan);

    projMatrix = mat.mat;

    UpdateCameraBuffer();
}


void VulkanShadowMapCamera::UpdateCameraBuffer()
{
    ShadowMapCameraBuffer bufferData;
    bufferData.view = viewMatrix;
    bufferData.proj = projMatrix;


    cameraBuffer.Update(&bufferData);
            
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = cameraBuffer.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(ShadowMapCameraBuffer);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = GetCameraDescriptorSet();
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), 1, &descriptorWrite, 0, nullptr);
}
