#ifndef IVULKANLIGHTMANAGER_H
#define IVULKANLIGHTMANAGER_H

#include "IVulkanLight.h"

/**
 * @brief The vulkan light manager's interface
 * @interface IVulkanLightManager
 * 
 * @author Vectem
 */
class IVulkanLightManager
{
public:
    virtual ~IVulkanLightManager() = default;
public:
    /**
     * @brief Add an light which be managed
     */
    virtual void AddLight(const IVulkanLight* light) = 0;

    /**
     * @brief Remove all managed lights to the manager
     */
    virtual void ClearLights() = 0;

    /**
     * @brief Update managed lights infos
     */
    virtual void Update() = 0;

    /**
     * @brief Bind and render lights
     */
    virtual void Bind(VkCommandBuffer commandBuffer) const = 0;
};

#endif // IVULKANLIGHTMANAGER_H