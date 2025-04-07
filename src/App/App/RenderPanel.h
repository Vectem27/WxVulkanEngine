#ifndef RENDERPANEL_H
#define RENDERPANEL_H

#include <wx/wx.h>
#include <vulkan/vulkan.h>
#include "Pipeline/VulkanOpaqueMaterial.h"
#include "VulkanRenderer.h"
#include "VulkanSwapchain.h"
#include "VulkanCamera.h"
#include "VulkanRenderTarget.h"
#include "World.h"
#include "Cube.h"
#include "CameraComponent.h"
#include "LightComponent.h"

class RenderPanel : public wxWindow
{
public:
    RenderPanel(wxWindow* parent) 
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
    {
        SetWindowLongPtr(GetHWND(), GWL_STYLE, GetWindowLongPtr(GetHWND(), GWL_STYLE) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        renderEngine = new VulkanRenderEngine();

        try
        {
            renderEngine->Init(reinterpret_cast<void *>(GetHandle()));
            
            surface = new VulkanSurface(renderEngine->GetInstance(), renderEngine->GetDeviceManager(), reinterpret_cast<void *>(GetHandle()));

            swapchain = new VulkanSwapchain(renderEngine, surface);
            swapchain->Create(renderEngine->GetDefaultRenderPass());

            renderTarget = new VulkanRenderTarget(renderEngine, 256, 256, renderEngine->GetDepthStencilImageFormat());
            renderTarget->CreateFramebuffer(renderEngine->GetShadowMapRenderPass());

            renderer = new VulkanRenderer(renderEngine);

            camera = new CameraComponent();
            light = new LightComponent();
            camera->VulkanCamera::Init(renderEngine, swapchain);
            light->VulkanLight::Init(renderEngine, renderTarget);
    
            cube = new Cube();
            cube->Init(renderEngine);
            tinyCube = new Cube();
            tinyCube->Init(renderEngine);

            
            material = new VulkanOpaqueMaterial(renderEngine->GetPipelineManager());
            MaterialInfo matInfo = {};
            matInfo.fragmentShader = "shaders/shader.frag";
            matInfo.vertexShader = "shaders/shader.vert";
            material->CreatePipelines(renderEngine->GetDevice(), renderEngine->GetDefaultRenderPass(), matInfo);
            
            MaterialInfo shadowMapMatInfo = {};
            shadowMapMatInfo.vertexShader = "shaders/shadowMap.vert";
            material->CreateShadowMapPipeline(renderEngine->GetDevice(), renderEngine->GetShadowMapRenderPass(), shadowMapMatInfo);


            cube->SetMaterial(material);
            tinyCube->SetMaterial(material);
            tinyCube->SetRelativePosition({1.0f,1.0f,1.0f});
            tinyCube->SetRelativeRotation(Rotator::FromEulerDegrees(0,0,45.0f));
            tinyCube->SetRelativeScale({.5f,.5f,.5f});

            world = new World();
            cubeActor = world->SpawnActor<Actor>();
            cubeActor->AddChild(cube);
            cubeActor->AddChild(tinyCube);
            world->SpawnActor<Actor>()->AddChild(camera);
            world->SpawnActor<Actor>()->AddChild(light);

            camera->SetRelativeTransform(Transform({0,0,5}, Quaternion<float>::FromEulerDegrees(-90,0,0), {1,1,1}));
            light->SetRelativeTransform(camera->GetRelativeTransform());
        }
        catch(const std::exception& e)
        {
            wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
        }


    }

    void Cleanup()
    {
        camera->Cleanup();
        delete camera;

        light->Cleanup();
        delete light;

        delete renderTarget;
        delete swapchain;
        delete renderer;
        delete world;

        if (renderEngine)
        {
            renderEngine->Shutdown();
            delete renderer;
        }
    }


    void Render()
    {
        static float pitch{.0f};
        static float roll{0.0f};
        static float yaw{.0f};
        //(pitch < 25.0f ? (yaw < 45.0f ? yaw : pitch) : roll) += 0.05f;

        static float yaw2{0.0f};
        yaw2 -= 0.5f;
        yaw += 0.1f;
        cubeActor->SetRelativeRotation(Rotator::FromEulerDegrees(pitch, roll, yaw));
        //camera->SetRelativeRotation(Rotator::FromEulerDegrees(pitch, roll, yaw));
        tinyCube->SetRelativeRotation(Rotator::FromEulerDegrees(0,0,yaw2));        
        try
        {
            //renderer->RenderToSwapchain(swapchain, world, camera, renderEngine->GetDeviceManager()->GetGraphicsQueue(), surface->GetPresentQueue());
            
            //return;
            renderer->RenderToShadowMap(renderTarget, world, light, renderEngine->GetDeviceManager()->GetGraphicsQueue());
            //** 
            // Après le rendu de la shadow map
            int w = renderTarget->GetWidth();
            int h = renderTarget->GetHeight();

            // Pour D32_SFLOAT (4 bytes par pixel)
            std::vector<float> depthData(w * h);
            renderTarget->CopyToCpuBuffer(depthData.data(), w * h * sizeof(float));
            // Création de l'image wxWidgets
            wxImage img(w, h);
            unsigned char* imgData = img.GetData();

            // Conversion des valeurs de profondeur en niveaux de gris
            for (int y = 0; y < h; y++) 
            {
                for (int x = 0; x < w; x++) 
                {
                    float depth = depthData[y * w + x ];
                    // Normalisation (inversée car les valeurs proches sont plus petites)
                    unsigned char gray = static_cast<unsigned char>((1.0f - depth) * 255);

                    gray = depth < 1.0f ? 0 : 255;
                    
                    // Remplir les 3 canaux (R,G,B) avec la même valeur
                    int idx = (y * w + x) * 3;
                    imgData[idx] = gray;
                    imgData[idx + 1] = gray;
                    imgData[idx + 2] = gray;
                }
            }

            // Affichage
            wxBitmap bmp(img);
            wxClientDC dc(this);
            dc.DrawBitmap(bmp, 0, 0, false);//*/
        }
        catch (const std::exception &e)
        {
            wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
        }
    }

public:
    VulkanRenderEngine *renderEngine;
    CameraComponent* camera;
    LightComponent* light;
    Actor* cubeActor;
    Cube* cube;
    Cube* tinyCube;
    VulkanOpaqueMaterial* material;
    VulkanRenderTarget* renderTarget;

    VulkanRenderer* renderer;
    VulkanSwapchain* swapchain;
    VulkanSurface* surface;
    World* world;
};


#endif // RENDERPANEL_H