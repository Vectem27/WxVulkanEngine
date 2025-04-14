#include "VulkanGlobalLightManager.h"
#include "VulkanRenderEngine.h"

#include "Logger.h"


void VulkanGlobalLightManager::AddLightManager(std::type_index lightType, IVulkanLightManager *lightManager)
{
    if (!lightManager)
    {
        Log(Warning, "Vulkan", "Can not add sub light manager to global light manager, sub light manager is null");
        return;
    }

    if (subManagers.contains(lightType))
        Log(Warning, "Vulkan", "Replacing an existing sub light manager with another whene add light manager to global light manager");

    subManagers.insert_or_assign(lightType, lightManager);
}

void VulkanGlobalLightManager::AddLight(const IVulkanLight *light)
{
    auto it = subManagers.find(typeid(*light));
    if (it == subManagers.end())
    {
        static uint8_t logLoopCount = 0;
        if (logLoopCount < 5)
            Log(Warning, "Vulkan", "Can't add light to global light manager, no suitable sub light manager. Light type id : %s", typeid(*light).name());

        ++logLoopCount;
        return;
    }

    it->second->AddLight(light);
}

void VulkanGlobalLightManager::ClearLights()
{
    for(auto& pair : subManagers)
        pair.second->ClearLights();
}

void VulkanGlobalLightManager::Update()
{
    for(auto& pair : subManagers)
        pair.second->Update();
}

void VulkanGlobalLightManager::Bind(VkCommandBuffer commandBuffer) const
{
    for(auto& pair : subManagers)
        pair.second->Bind(commandBuffer);
}
