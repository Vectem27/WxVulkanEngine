#ifndef VULKANSCENERENDERER_H
#define VULKANSCENERENDERER_H

// Standard
#include <vector>

// Scene
#include "SceneComponent.h"
#include "Array.h"
#include "IVulkanMesh.h"

// Renderer
#include "VulkanShadowMapRenderer.h"
#include "VulkanRenderTargetRenderer.h"

// Lighting
#include "ILightComponent.h"
#include "VulkanSpotlightLight.h"
#include "VulkanGlobalLightManager.h"
#include "LightManagers/VulkanSpotlightLightManager.h"



class VulkanSceneRenderer
{
public:
    VulkanSceneRenderer() = default;
    ~VulkanSceneRenderer() = default;

    void Init()
    {
        auto spotlightLightManager = new VulkanSpotlightLightManager();
        lightManager.AddLightManager(typeid(VulkanSpotlightLight), spotlightLightManager);
    }
   
    void RenderScene(SceneComponent* scene, VulkanCamera* camera)
    {

        Array<SceneComponent*> sceneArray;
        if (scene)
            scene->CollectChilds(sceneArray);

        std::vector<IVulkanMesh*> meshes;
        Array<IVulkanLight*> sceneLights;

        lightManager.ClearLights();
        for (auto& object : sceneArray)
        {
            auto lightComponent = dynamic_cast<ILightComponent*>(object);
            if(lightComponent)
            {
                auto light = static_cast<IVulkanLight*>(lightComponent->GetRenderLight(RenderAPI::Vulkan));
                if(light)
                    sceneLights.Add(light);
            }

            auto mesh = reinterpret_cast<IVulkanMesh*>(object->GetRenderMesh());
            if(mesh)
                meshes.push_back(mesh);
        }

        for (auto& light : sceneLights)
        {
            lightManager.AddLight(light);
            auto cams = light->GetShadowMapCameras();
            for (auto& cam : cams)
                GetVulkanShadowMapRenderer().Render(cam, meshes.data(), meshes.size());
        }

        static bool doOnce{true};
        if (doOnce)
        {
            lightManager.Update();
            doOnce = false;
        }

        GetVulkanRenderTargetRenderer().Render(dynamic_cast<IVulkanRenderTarget*>(camera->GetRenderTarget()), camera, meshes.data(), meshes.size(), lightManager);
    }

    void RenderWorld(World* world, VulkanCamera* camera)
    {

        Array<SceneComponent*> sceneArray;
        for (auto& actor : world->GetActors())
        {
            if (actor)
                actor->CollectChilds(sceneArray);
        }

        std::vector<IVulkanMesh*> meshes;
        Array<IVulkanLight *> sceneLights;

        lightManager.ClearLights();
        for (auto& object : sceneArray)
        {
            auto lightComponent = dynamic_cast<ILightComponent*>(object);
            if(lightComponent)
            {
                auto light = static_cast<IVulkanLight*>(lightComponent->GetRenderLight(RenderAPI::Vulkan)); 
                if(light)
                    sceneLights.Add(light);
            }

            auto mesh = reinterpret_cast<IVulkanMesh*>(object->GetRenderMesh());
            if(mesh)
                meshes.push_back(mesh);
        }

        for (auto& light : sceneLights)
        {
            lightManager.AddLight(light);
            auto cams = light->GetShadowMapCameras();
            for (auto& cam : cams)
                GetVulkanShadowMapRenderer().Render(cam, meshes.data(), meshes.size());
        }

        static bool doOnce{true};
        if (doOnce)
        {
            lightManager.Update();
            doOnce = false;
        }

        GetVulkanRenderTargetRenderer().Render(dynamic_cast<IVulkanRenderTarget*>(camera->GetRenderTarget()), camera, meshes.data(), meshes.size(), lightManager);
    }


private:
    VulkanGlobalLightManager lightManager;
};

#endif // VULKANSCENERENDERER_H
