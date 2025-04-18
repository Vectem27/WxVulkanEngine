#ifndef VULKANLIGHTMANAGER_H
#define VULKANLIGHTMANAGER_H

#include <unordered_map>
#include <typeindex>

#include <vulkan/vulkan.h>

#include "LightManagers/IVulkanLightManager.h"

/**
 * @brief A global light manager which can group other light managers
 * @class VulkanGlobalLightManager
 * @implements IVulkanLightManager
 * 
 * @author Vectem
 */
class VulkanGlobalLightManager : public IVulkanLightManager
{
public:
    /**
     * @brief Function to add single light manager
     * @param lightType Is the light type the manager manage
     * @param lightManager The light manager pointer
     * 
     * Example of use : 
     * @code 
     * globalManager.AddLightManager(typeid(VulkanPointLight), &pointLightManager);
     * @endcode
     */
    void AddLightManager(std::type_index lightType, IVulkanLightManager* lightManager);

public: // IVulkanLightManager Interface
    virtual void AddLight(const IVulkanLight* light) override;
    virtual void ClearLights() override;

    virtual void Update() override;
    virtual void Bind(VkCommandBuffer commandBuffer) const override;

private:
    std::unordered_map<std::type_index, IVulkanLightManager*> subManagers;
};

#endif // VULKANLIGHTMANAGER_H