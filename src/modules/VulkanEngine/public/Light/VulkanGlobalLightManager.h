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

class VulkanSpotlightLight;
class IVulkanLightManager;

class VulkanGlobalLightManager : public IVulkanLightManager
{
public:
    bool AddLightManager(VulkanLightType lightType, IVulkanLightManager* lightManager);
    IVulkanLightManager* GetManager(VulkanLightType lightType) const;

public:
    //void InitLightManager();

    virtual void AddLight(const IVulkanLight* light) override;
    virtual void ClearLights() override;

    virtual void Update() override;
    virtual void Bind(VkCommandBuffer commandBuffer) const override;

private:
    Array<LightManager> managers;
};

#endif // VULKANLIGHTMANAGER_H