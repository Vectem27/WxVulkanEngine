#ifndef VULKANSHADOWMAPCAMERA_H
#define VULKANSHADOWMAPCAMERA_H

#include <inttypes.h>

#include <vulkan/vulkan.h>

#include "EngineCore.hpp"
#include "VulkanUniformBuffer.h"

class IVulkanShadowMap;

class VulkanShadowMapCamera
{
public:
    VulkanShadowMapCamera();
    ~VulkanShadowMapCamera();

    IVulkanShadowMap* GetShadowMapTarget() const { return shadowMap; }

    Matrix4f GetViewMatrix() const { return viewMatrix; }
    Matrix4f GetProjMatrix() const { return projMatrix; }

    void SetShadowMap(IVulkanShadowMap* shadowMap);
    void SetTransform(Transform transform);
    void SetFOV(float fov);
    void SetNearPlan(float nearPlan);
    void SetFarPlan(float farPlan);

    void Bind(VkCommandBuffer commandBuffer) const;

private:
    VkDescriptorPool& GetCameraDescriptorPool() { return descriptorPool; }
    VkDescriptorSet& GetCameraDescriptorSet() { return descriptorSet; }
    const VkDescriptorSet& GetCameraDescriptorSet() const { return descriptorSet; }

private:
    void UpdateProjectionMatrix();
    void UpdateViewMatrix();

    void UpdateCameraBuffer();
private:
    IVulkanShadowMap* shadowMap{nullptr};

    const float aspectRatio{1.0f};
    float nearPlan{0.1f};
    float farPlan{100.0f};
    float fov{0.785398};

    Transform transform;

    Matrix4f viewMatrix;
    Matrix4f projMatrix;

    VulkanUniformBuffer cameraBuffer;

    VkDescriptorPool descriptorPool{VK_NULL_HANDLE};
    VkDescriptorSet descriptorSet{VK_NULL_HANDLE};
};

#endif // VULKANSHADOWMAPCAMERA_H