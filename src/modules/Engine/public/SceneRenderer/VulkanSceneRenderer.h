#ifndef VULKANSCENERENDERER_H
#define VULKANSCENERENDERER_H

#include "VulkanShadowMapRenderer.h"
#include "VulkanRenderTargetRenderer.h"

#include "SceneComponent.h"
#include "VulkanCamera.h"
#include "Array.h"
#include <vector>
#include "IVulkanMesh.h"

#include "VulkanGlobalLightManager.h"
#include "LightManagers/VulkanSpotlightLightManager.h"

//TODO: Adding light managers inside the class

class VulkanSceneRenderer
{
public:
    VulkanSceneRenderer() = default;
    ~VulkanSceneRenderer() = default;

    void Init()
    {
        auto spotlightLightManager = new VulkanSpotlightLightManager(16);
        spotlightLightManager->InitLightManager();
        lightManager.AddLightManager(VulkanSpotlightLight::lightType, spotlightLightManager);
    }
   
    void RenderScene(SceneComponent* scene, VulkanCamera* camera)
    {

        Array<SceneComponent*> sceneArray;
        if (scene)
            scene->CollectChilds(sceneArray);

        std::vector<IVulkanMesh*> meshes;
        Array<VulkanSpotlightLight*> sceneLights;

        lightManager.ClearLights();
        for (auto& object : sceneArray)
        {
            auto light = dynamic_cast<VulkanSpotlightLight*>(object);
            if(light)
                sceneLights.Add(light);

            auto mesh = reinterpret_cast<IVulkanMesh*>(object->GetRenderMesh());
            if(mesh)
                meshes.push_back(mesh);
        }

        for (auto& light : sceneLights)
        {
            lightManager.AddLight(light);
            //RenderToShadowMap(light->renderTarget, renderObject, light->camera, graphicsQueue);
            GetVulkanShadowMapRenderer().Render(light->renderTarget, light->camera, meshes.data(), meshes.size());
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
        Array<VulkanSpotlightLight*> sceneLights;

        lightManager.ClearLights();
        for (auto& object : sceneArray)
        {
            auto light = dynamic_cast<VulkanSpotlightLight*>(object);
            if(light)
                sceneLights.Add(light);

            auto mesh = reinterpret_cast<IVulkanMesh*>(object->GetRenderMesh());
            if(mesh)
                meshes.push_back(mesh);
        }

        for (auto& light : sceneLights)
        {
            lightManager.AddLight(light);
            //RenderToShadowMap(light->renderTarget, renderObject, light->camera, graphicsQueue);
            GetVulkanShadowMapRenderer().Render(light->renderTarget, light->camera, meshes.data(), meshes.size());
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
