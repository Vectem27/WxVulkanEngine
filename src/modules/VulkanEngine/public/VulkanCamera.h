#ifndef VULKANCAMERA_H
#define VULKANCAMERA_H

#include "ICamera.h"
#include <vulkan/vulkan.h>
#include "VulkanUniformBuffer.h"
#include "EngineCore.hpp"

class VulkanCamera : public ICamera
{
    struct ViewProj
    {
        alignas(16) Matrix4f view;
        alignas(16) Matrix4f proj;
    };
public:
    VulkanCamera() {}
    ~VulkanCamera() {}

public: // ICamera Interface
    virtual bool Init(class IRenderEngine* renderEngine, class IRenderTarget* renderTarget = nullptr) override;
    virtual bool Render(class IRenderable* renderable, const VkCommandBuffer& commandBuffer);
    virtual void Cleanup() override;
    virtual class IRenderTarget* GetRenderTarget() const override;
    virtual void SetRenderTarget(class IRenderTarget* renderTarget) override;

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
    class VulkanRenderEngine* renderEngine;
    class IRenderTarget* renderTarget;

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