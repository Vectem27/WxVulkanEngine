#ifndef VULKANLIGHT_H
#define VULKANLIGHT_H

#include "ICamera.h"
#include <vulkan/vulkan.h>
#include "UniformBufferObject.h"
#include "Vertex.h"
#include "EngineCore.hpp"
#include <glm/glm.hpp>

class VulkanLight : public ICamera
{
public:
    VulkanLight() = default;
    ~VulkanLight() = default;

public: // ICamera Interface
    virtual bool Init(class IRenderEngine* renderEngine, class IRenderTarget* renderTarget = nullptr) override;
    virtual bool Render(class IRenderable* renderable, const VkCommandBuffer& commandBuffer);
    virtual void Cleanup() override;
    virtual class IRenderTarget* GetRenderTarget() const override;
    virtual void SetRenderTarget(class IRenderTarget* renderTarget) override;

public:
    VkBuffer GetViewDataBuffer() const { return viewBuffer.GetBuffer(); }
    const VkDescriptorSet* GetDescriptorSet() const { return &lightDescriptorSet; }
    const VkDescriptorSet* GetLightDescriptorSet() const { return &lightDescriptorSet; }

    void SetAspectRatio(float aspectRatio);
    void SetFOV(float fov); // Y fov in degrees
    void SetNearPlan(float nearPlan);
    void SetFarPlan(float farPlan);
    void SetShadowMap(VkImageView shadowMapView, VkSampler shadowMapSampler);

protected:
    void SetLightTransform(Transform transform);

private: 
    class VulkanRenderEngine* renderEngine = nullptr;
    class IRenderTarget* renderTarget = nullptr;

    // Descriptors
    VkDescriptorSet lightDescriptorSet = VK_NULL_HANDLE;
    VkDescriptorPool lightDescriptorPool = VK_NULL_HANDLE;

    UniformBuffer viewBuffer;

    // Data
    ViewProj viewData;

private: // Initialization
    void CreateDescriptorPool();
    void CreateDescriptors();

private: // Updates
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void UpdateViewDataBuffer();

private: // Camera parameters
    float aspectRatio = 1.0f;
    float nearPlan = 0.1f;
    float farPlan = 1000.0f;
    float fov = 103.0f; // Y fov in degrees
};

#endif // VULKANLIGHT_H