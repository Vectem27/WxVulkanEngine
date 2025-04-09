#ifndef VULKANLIGHTMANAGER_H
#define VULKANLIGHTMANAGER_H

#include <vulkan/vulkan.h>
#include "EngineCore.hpp"
#include "IVulkanLight.h"
#include "LightManagers/IVulkanLightManager.h"

struct LightManager
{
    VulkanLightType lightType;
    IVulkanLightManager* manager;
};

class VulkanRenderEngine;
class VulkanSpotlightLight;
class IVulkanLightManager;

class VulkanGlobalLightManager : public IVulkanLightManager
{
public:
    bool AddLightManager(VulkanLightType lightType, IVulkanLightManager* lightManager);
    IVulkanLightManager* GetManager(VulkanLightType lightType) const;
    VulkanRenderEngine* GetRenderEngine() { return vulkanRenderEngine; }

public:
    virtual void InitLightManager(VulkanRenderEngine* vulkanRenderEngine) override;
    virtual bool IsInitialized() const override { return isInitialized; }

    virtual void AddLight(const IVulkanLight* light) override;
    virtual void ClearLights() override;

    virtual void Update() override;
    virtual void Bind(VkCommandBuffer commandBuffer) const override;

private:
    bool isInitialized{false};
    VulkanRenderEngine* vulkanRenderEngine;

    Array<LightManager> managers;
};

#endif // VULKANLIGHTMANAGER_H