#ifndef VULKANSPOTLIGHTLIGHTMANAGER_H
#define VULKANSPOTLIGHTLIGHTMANAGER_H

#include <vulkan/vulkan.h>

#include "Array.h"

#include "IVulkanLightManager.h"
#include "VulkanUniformBuffer.h"

class VulkanSpotlightLight;
class VulkanSpotlightLightPipeline;

/**
 * @brief The spotlight lights manager
 * @class VulkanSpotlightLightManager
 * @implements IVulkanLightManager
 * 
 * @author Vectem
 */
class VulkanSpotlightLightManager : public IVulkanLightManager
{
public:
    /**
     * @brief Default constructor
     * @warning Class should be created after vulkan engine singletons initializations
     */
    VulkanSpotlightLightManager();
    VulkanSpotlightLightManager(const VulkanSpotlightLightManager&) = delete;
    VulkanSpotlightLightManager& operator=(const VulkanSpotlightLightManager&) = delete;
    VulkanSpotlightLightManager(VulkanSpotlightLightManager&&) = delete;
    VulkanSpotlightLightManager& operator=(VulkanSpotlightLightManager&&) = delete;

    virtual ~VulkanSpotlightLightManager();

public: // IVulkanLightManager Interface
    virtual void AddLight(const IVulkanLight* light) override;
    virtual void ClearLights() override { lights.Clear(); }
    virtual void Update() override;
    virtual void Bind(VkCommandBuffer commandBuffer) const override;

public:
    /**
     * @brief Get the current number of managed lights
     * @return the number of lights
     */
    unsigned short GetLightCount() const { return lights.GetSize();}

private:
    Array<const VulkanSpotlightLight*> lights;

    VulkanSpotlightLightPipeline* spotlightLightPipeline{nullptr};

    VulkanUniformBuffer lightDataBuffer;
    VkDescriptorSet descriptorSet;

    VkDescriptorPool descriptorPool;
};

#endif // VULKANSPOTLIGHTLIGHTMANAGER_H