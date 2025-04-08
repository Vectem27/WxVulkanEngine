#ifndef VULKANLIGHTMANAGER_H
#define VULKANLIGHTMANAGER_H

#include <vulkan/vulkan.h>
#include "EngineCore.hpp"

struct VertexLightData
{
    alignas(16) Matrix4f viewProj;
};

struct FragmentLightData
{
    alignas(16) Vector3f pos;
    alignas(16) Vector3f direction;
    alignas(4) float length;
    alignas(4) float angle;
};

class VulkanRenderEngine;
class VulkanSpotlightLight;

class VulkanLightManager
{
public:

    void InitVulkanLightManager(VulkanRenderEngine* vulkanRenderEngine);

    bool IsInitialized() const { return isInitialized; }

    void ClearLights();

    void UpdateDescriptorSets();
    const VkDescriptorSet& GetProjectorLightsDescriptorSet() const { return projectorLightsDescriptorSet; }

    VulkanRenderEngine* GetRenderEngine() const { return vulkanRenderEngine; }
public:
    void AddProjectorLight(const VulkanSpotlightLight* light);
private:
    void CreateDescriptorPool();
    void CreateDescriptorSets();
private:
    bool isInitialized{false};
    VulkanRenderEngine* vulkanRenderEngine;

    VkDescriptorPool projectorLightsDescriptorPool;
    VkDescriptorSet projectorLightsDescriptorSet;

    Array<const VulkanSpotlightLight*> projectorLights;
};

#endif // VULKANLIGHTMANAGER_H