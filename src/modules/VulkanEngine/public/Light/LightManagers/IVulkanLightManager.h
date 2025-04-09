#ifndef IVULKANLIGHTMANAGER_H
#define IVULKANLIGHTMANAGER_H

#include "IVulkanLight.h"

class VulkanRenderEngine;

class IVulkanLightManager
{
public:
    virtual void InitLightManager(VulkanRenderEngine* vulkanRenderEngine) = 0;
    virtual bool IsInitialized() const = 0;

    virtual void AddLight(const IVulkanLight* light) = 0;
    virtual void ClearLights() = 0;

    virtual void Update() = 0;
    virtual void Bind(VkCommandBuffer commandBuffer) const = 0;
};

#endif // IVULKANLIGHTMANAGER_H