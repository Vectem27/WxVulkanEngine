#ifndef VULKANCAMERA_H
#define VULKANCAMERA_H

#include "ICamera.h"
#include <vulkan/vulkan.h>
#include "UniformBufferObject.h"
#include "Vertex.h"

class VulkanCamera : public ICamera
{
public:
    VulkanCamera() {}
    ~VulkanCamera() {}

public: // ICamera Interface
    virtual bool Init(class IRenderEngine* renderEngine, class IRenderTarget* renderTarget = nullptr) override;
    virtual bool Render(class IRenderable* renderable) override;
    virtual void Cleanup() override;
    virtual class IRenderTarget* GetRenderTarget() const override;
    virtual void SetRenderTarget(class IRenderTarget* renderTarget) override;

public:
    VkBuffer GetViewDataBuffer() const { return viewBuffer.GetBuffer(); }
    VkBuffer GetObjectDataBuffer() const { return objectBuffer.GetBuffer(); }
    const VkDescriptorSet* GetDescriptorSet() const { return &cameraDescriptorSet; }

    void SetAspectRatio(float aspectRatio);
    void SetFOV(float fov);
    void SetNearPlan(float nearPlan);
    void SetFarPlan(float farPlan);
private: 
    class VulkanRenderer* renderEngine;
    class IRenderTarget* renderTarget;

    VkDescriptorSet cameraDescriptorSet{VK_NULL_HANDLE};
    VkDescriptorPool cameraDescriptorPool{VK_NULL_HANDLE};

    UniformBuffer viewBuffer;
    TransformMVP viewData;

    UniformBuffer objectBuffer;
    ObjectData objectData;
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
    // Y fov in degree
    float fov {45.0f};

};
#endif // VULKANCAMERA_H