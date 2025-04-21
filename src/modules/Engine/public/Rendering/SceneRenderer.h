#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "VulkanSceneRenderer.h"
#include "Engine.h"
#include "ICameraComponent.h"
#include "SceneComponent.h"
#include "World.h"

#include "IRenderable.h"

class SceneRenderer 
{
public:
    SceneRenderer()
    {
        switch (GetEngine().GetRenderAPI())
        {
        case Render:
            vulkanSceneRenderer = new VulkanSceneRenderer();
            vulkanSceneRenderer->Init();
            break;
        default:
            Log(Error, "LogRendering", "RenderAPI not supported");
            break;
        }
    }
    ~SceneRenderer()
    {
        if (vulkanSceneRenderer)
        {
            delete vulkanSceneRenderer;
            vulkanSceneRenderer = nullptr;
        }
    }

    void Render(World* world, ICameraComponent* camera, const RHI::IRenderable* testRenderable = nullptr)
    {
        switch (GetEngine().GetRenderAPI())
        {
        case RenderAPI::Vulkan:
            if (vulkanSceneRenderer)
            {
                vulkanSceneRenderer->RenderScene(
                    world, static_cast<VulkanCamera*>(camera->GetRenderCamera(VULKAN_API))
                    [](Vulkan::RenderInfo renderInfo){
                        if(testRenderable)
                            testRenderable->Render(renderInfo);
                    }
                );
            }
            break;
        default:
            Log(Error, "LogRendering", "RenderAPI not supported");
            break;
        }
    }

private:
    VulkanSceneRenderer* vulkanSceneRenderer{nullptr};
    
};

#endif // SCENERENDERER_H