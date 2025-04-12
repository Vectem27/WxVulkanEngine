#include "VulkanGlobalLightManager.h"
#include "VulkanRenderEngine.h"

bool VulkanGlobalLightManager::AddLightManager(VulkanLightType lightType, IVulkanLightManager *lightManager)
{
    if (GetManager(lightType))
        return false;

    managers.Add({lightType, lightManager});

    return true;
}

IVulkanLightManager *VulkanGlobalLightManager::GetManager(VulkanLightType lightType) const
{
    for (const auto& manager : managers)
    {
        if (manager.lightType == lightType)
            return manager.manager;
    }

    return nullptr;
}

void VulkanGlobalLightManager::AddLight(const IVulkanLight *light)
{
    auto manager = GetManager(light->GetLightType());
    if(!manager)
        return;

    manager->AddLight(light);
}

void VulkanGlobalLightManager::ClearLights()
{
    for (const auto& manager : managers)
        manager.manager->ClearLights();
}

void VulkanGlobalLightManager::Update()
{
    for (const auto& manager : managers)
        manager.manager->Update();
}

void VulkanGlobalLightManager::Bind(VkCommandBuffer commandBuffer) const
{
    for (const auto& manager : managers)
        manager.manager->Bind(commandBuffer);
}
