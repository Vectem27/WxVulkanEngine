#include "VulkanLight.h"
#include "IRenderable.h"
#include "ICamera.h"
#include "VulkanRenderEngine.h"
#include "IRenderTarget.h"
#include <array>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "EngineCore.hpp"

bool VulkanLight::Init(IRenderEngine* renderEngine, class IRenderTarget* renderTarget)
{
    this->renderEngine = dynamic_cast<VulkanRenderEngine*>(renderEngine);
    if (!this->renderEngine)
    {
        return false;
    }

    // Init RenderPass
    CreateDescriptorPool();
    CreateDescriptors();

    if (renderTarget)
        SetRenderTarget(renderTarget);

    // Création des buffers pour les données de lumière
    viewBuffer.Create(this->renderEngine->GetDevice(), this->renderEngine->GetPhysicalDevice(), sizeof(ViewProj));

    SetLightTransform(Transform());
    UpdateProjectionMatrix();

    return true;
}

bool VulkanLight::Render(IRenderable* renderable, const VkCommandBuffer& commandBuffer)
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

    vkCmdBindDescriptorSets(
        commandBuffer, 
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        renderEngine->GetPipelineManager()->GetPipelineLayout(), 
        0, 1, 
        GetDescriptorSet(), 
        0, nullptr
    );

    return true;
}

void VulkanLight::Cleanup()
{
    viewBuffer.Cleanup();

    if (lightDescriptorPool != VK_NULL_HANDLE) 
    {
        vkDestroyDescriptorPool(renderEngine->GetDevice(), lightDescriptorPool, nullptr);
        lightDescriptorPool = VK_NULL_HANDLE;
    }
}

void VulkanLight::SetRenderTarget(IRenderTarget* renderTarget)
{
    this->renderTarget = renderTarget;
}

void VulkanLight::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void VulkanLight::SetFOV(float fov)
{
    this->fov = fov;
    UpdateProjectionMatrix();
}

void VulkanLight::SetNearPlan(float nearPlan)
{
    this->nearPlan = nearPlan;
    UpdateProjectionMatrix();
}

void VulkanLight::SetFarPlan(float farPlan)
{
    this->farPlan = farPlan;
    UpdateProjectionMatrix();
}

void VulkanLight::SetLightTransform(Transform transform)
{
    auto r = transform.rotation.ToEuler(ERS_ZYX);
    transform.rotation = Rotator::FromEuler(r.x, -r.y, -r.z, ERS_XYZ);

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

IRenderTarget* VulkanLight::GetRenderTarget() const 
{ 
    return renderTarget; 
}

void VulkanLight::CreateDescriptorPool()
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 10; // Gérer plusieurs materials

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 10;

    if (vkCreateDescriptorPool(renderEngine->GetDevice(), &poolInfo, nullptr, &lightDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Échec de la création du descriptor pool !");
    }
}

void VulkanLight::CreateDescriptors()
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &renderEngine->GetPipelineManager()->GetCameraDescriptorSetLayout();

    if (!renderEngine->GetDescriptorPoolManager()->AllocateDescriptorSets(&renderEngine->GetPipelineManager()->GetCameraDescriptorSetLayout(), 1, &lightDescriptorSet))
    {
        throw std::runtime_error("Échec de l'allocation du descriptor set !");
    }
}

void VulkanLight::SetShadowMap(VkImageView shadowMapView, VkSampler shadowMapSampler)
{
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    imageInfo.imageView = shadowMapView;
    imageInfo.sampler = shadowMapSampler;

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = lightDescriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(renderEngine->GetDevice(), 1, &descriptorWrite, 0, nullptr);
}

void VulkanLight::UpdateViewMatrix()
{
    UpdateViewDataBuffer();
}

void VulkanLight::UpdateProjectionMatrix()
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

void VulkanLight::UpdateViewDataBuffer()
{
    auto mat = viewData.view * viewData.proj;
    viewBuffer.Update(&mat);
            
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = viewBuffer.GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(ViewProj);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = lightDescriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(renderEngine->GetDevice(), 1, &descriptorWrite, 0, nullptr);
}

#undef GLM_ENABLE_EXPERIMENTAL