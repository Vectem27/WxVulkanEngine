#ifndef VULKANCAMERA_H
#define VULKANCAMERA_H

#include "ICamera.h"
#include <vulkan/vulkan.h>
#include "VulkanUniformBuffer.h"
#include "EngineCore.hpp"

#include "logger.h"

class VulkanCamera
{
    struct ViewProj
    {
        alignas(16) Matrix4f view;
        alignas(16) Matrix4f proj;
    };
public:
    VulkanCamera() {}
    virtual ~VulkanCamera() { Cleanup(); }

public: // ICamera Interface
    void Init(class IVulkanRenderTarget* renderTarget = nullptr);
    virtual bool Render(class IRenderable* renderable, const VkCommandBuffer& commandBuffer);
    void Cleanup();
    virtual class IVulkanRenderTarget* GetRenderTarget() const;
    virtual void SetRenderTarget(class IVulkanRenderTarget* renderTarget);

public:
    VkBuffer GetViewDataBuffer() const { return viewBuffer.GetBuffer(); }
    const VkDescriptorSet* GetDescriptorSet() const { return &cameraDescriptorSet; }

    void SetAspectRatio(float aspectRatio);
    /* Y fov in degree*/
    void SetFOV(float fov);
    void SetNearPlan(float nearPlan);
    void SetFarPlan(float farPlan);

    ViewProj GetViewData() const { return viewData; }
public:
    void SetCameraTransform(Transform transform);
private: 
    class IVulkanRenderTarget* renderTarget;

    VkDescriptorSet cameraDescriptorSet{VK_NULL_HANDLE};
    VkDescriptorPool cameraDescriptorPool{VK_NULL_HANDLE};

    VulkanUniformBuffer viewBuffer;
    ViewProj viewData;
private: // Create
    void CreateDescriptorPool();
    void CreateDescriptors();

private: // Update
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void UpdateViewDataBuffer();

private:
    // View
    float aspectRatio {1.0f};
    float nearPlan {0.1f};
    float farPlan {1000.0f};
    /* Y fov in degree*/
    float fov {103.0f};

};
#endif // VULKANCAMERA_H