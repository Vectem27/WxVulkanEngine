#ifndef VULKANLIGHTMANAGER_H
#define VULKANLIGHTMANAGER_H

#include <vulkan/vulkan.h>
#include "EngineCore.hpp"

struct LightData
{
    alignas(16) Matrix4f viewProj;
};

class VulkanRenderEngine;
class VulkanProjectorLight;

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
    void AddProjectorLight(const VulkanProjectorLight* light);
private:
    void CreateDescriptorPool();
    void CreateDescriptorSets();
private:
    bool isInitialized{false};
    VulkanRenderEngine* vulkanRenderEngine;

    VkDescriptorPool projectorLightsDescriptorPool;
    VkDescriptorSet projectorLightsDescriptorSet;

    Array<const VulkanProjectorLight*> projectorLights;
};

#endif // VULKANLIGHTMANAGER_H