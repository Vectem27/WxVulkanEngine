#include "App.h"

#include "Pipeline/VulkanOpaqueMaterial.h"
#include "VulkanSwapchain.h"
#include "VulkanCamera.h"
#include "World.h"
#include "Cube.h"
#include "CameraComponent.h"
#include "VulkanGlobalLightManager.h"
#include "ProjectorLightComponent.h"
#include "LightManagers/VulkanSpotlightLightManager.h"

#include "VulkanRenderPassManager.h"

#include "VulkanSceneRenderer.h"

wxVulkanApp::wxVulkanApp()
    : frame(new wxVulkanFrame("Vulkan avec wxWidgets")), 
    vulkanRenderEngine(new VulkanRenderEngine()),
    camera(new CameraComponent())

{
}

bool wxVulkanApp::OnInit()
{

    Logger::GetLogger().SetVerbosity(3);

    Log(Info, LogDefault, "Application initialization start");

    frame->Show(true);
    
    Bind(wxEVT_IDLE, &wxVulkanApp::onIdle, this);
    InitVulkan();

    frame->SetSliderChangeCallback([this](int yaw){
        if (cameraActor)
        {
            auto rot = cameraActor->GetRelativeTransform().rotation.ToEulerDegrees();
            cameraActor->SetRelativeRotation(Rotator::FromEulerDegrees(rot.x, rot.y, static_cast<float>(yaw)));
        }
    });

    frame->SetSlider2ChangeCallback([this](int pitch){
        if (cameraActor)
        {
            auto rot = cameraActor->GetRelativeTransform().rotation.ToEulerDegrees();
            cameraActor->SetRelativeRotation(Rotator::FromEulerDegrees(static_cast<float>(pitch), rot.y, rot.z));
        }
    });

    Log(Info, LogDefault, "Application initialization finished");

    return true;
}

void wxVulkanApp::onIdle(wxIdleEvent &evt)
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - lastFrameTime;
    lastFrameTime = now;

    fps = 1.0 / elapsed.count(); // Calcul des FPS
    frame->SetTitle(wxString::Format("Vulkan avec wxWidgets - FPS: %.0f", fps));

    RenderVulkan();
    evt.RequestMore();
}

int wxVulkanApp::OnExit()
{
    Log(Info, LogDefault, "Application shutdown");
    ShutdownVulkan();
    Log(Info, LogDefault, "Application exit");
    return wxApp::OnExit();
}

void wxVulkanApp::InitVulkan()
{
    try
    {
        vulkanRenderEngine->Init(nullptr);
        
        frame->renderSurface->InitVulkanSurface(vulkanRenderEngine);
        
        if (!frame->renderSurface->IsVulkanInitialized())
            throw std::runtime_error("Render surface not initialized");
        swapchain = new VulkanSwapchain(vulkanRenderEngine, frame->renderSurface->GetVulkanSurface());
        swapchain->Create(
            VulkanRenderPassManager::GetInstance()->GetGeometryPass()
        );

        auto projLight2 = new SpotlightLightComponent();
        projLight = new SpotlightLightComponent();
        camera->VulkanCamera::Init(swapchain);

        cube = new Cube();
        cube->Init(vulkanRenderEngine);
        tinyCube = new Cube();
        tinyCube->Init(vulkanRenderEngine);
        floor = new Cube();
        floor->Init(vulkanRenderEngine);
        
        material = new VulkanOpaqueMaterial(vulkanRenderEngine->GetPipelineManager());
        MaterialInfo matInfo = {};
        matInfo.fragmentShader = "shaders/shader.frag";
        matInfo.vertexShader = "shaders/shader.vert";
        matInfo.shadowMapVertexShader = "shaders/shadowMap.vert";
        matInfo.lightingFragmentShader = "shaders/lighting.vert";
        matInfo.lightingVertexShader= "shaders/lighting.vert";
        material->CreatePipelines(
            GetVulkanDeviceManager().GetDeviceChecked(), 
            VulkanRenderPassManager::GetInstance()->GetGeometryPass(), 
            matInfo
        );

        material->CreateShadowMapPipeline(
            GetVulkanDeviceManager().GetDeviceChecked(), 
            VulkanRenderPassManager::GetInstance()->GetShadowPass(), 
            matInfo
        );

        cube->SetMaterial(material);
        tinyCube->SetMaterial(material);
        tinyCube->SetRelativePosition({1.0f,1.0f,1.0f});
        tinyCube->SetRelativeRotation(Rotator::FromEulerDegrees(0,0,45.0f));
        tinyCube->SetRelativeScale({.5f,.5f,.5f});

        floor->SetMaterial(material);
        floor->SetRelativePosition({0.0f,0.0f,-1.0f});
        floor->SetRelativeScale({15.f,15.0f,1.0f});

        // TODO: Check why it crashed whene dont spawn cubes
        world = new World();
        cubeActor = world->SpawnActor<Actor>();
        cubeActor->AddChild(cube);
        cubeActor->AddChild(tinyCube);
        world->SpawnActor<Actor>()->AddChild(floor);
        cameraActor = world->SpawnActor<Actor>();
        cameraActor->AddChild(camera);
        Actor* li = world->SpawnActor<Actor>();
        Actor* li2 = world->SpawnActor<Actor>();
        li->AddChild(projLight);
        projLight->SetLightSoftAngle(5.0f);
        li2->AddChild(projLight2);
        projLight2->SetLightColor({0.2f, 0.8f, 0.5f});
        projLight2->SetLightAngle(15.0f);
        projLight2->SetLightSoftAngle(15.0f);

        camera->SetRelativeTransform(Transform({-5,-2,2}, Rotator::FromEulerDegrees(-10,0,45), {1,1,1}));
        li->SetRelativeTransform(Transform({-6,6,4}, Rotator::FromEulerDegrees(-30,0,-45), {1,1,1}));
        li2->SetRelativeTransform(Transform({-3,3,8}, Rotator::FromEulerDegrees(-80,0,45), {1,1,1}));
        camera->SetFOV(103.f / 2.0f);


        sceneRenderer = new VulkanSceneRenderer();
        sceneRenderer->Init();
    }
    catch(const std::exception& e)
    {
        wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
        ShutdownVulkan();
        exit(1);
    }
}

void wxVulkanApp::RenderVulkan()
{
    static unsigned short counter{0};
    
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
     
        if(frame->renderSurface->IsVulkanInitialized())
        {
            sceneRenderer->RenderWorld(world, camera);
            swapchain->Present();
        }
        return;
        ++counter;
        if (counter % 5 != 0)
            return;
            
        /** 
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
        bmp.Rescale(bmp, wxSize(96,96));
        wxClientDC dc(frame->testPanel);
        dc.DrawBitmap(bmp, 0, 0, false);*/
    }
    catch (const std::exception &e)
    {
        wxMessageBox(e.what(), "Erreur", wxICON_ERROR);
        ShutdownVulkan();
        exit(1);
    }
}

void wxVulkanApp::ShutdownVulkan()
{
    Log(Debug, LogDefault, "Shutdown vulkan");

    Log(Trace, LogDefault, "Destroy world");
    delete world;
    
    Log(Trace, LogDefault, "Destroy swapchain");
    delete swapchain;

    Log(Trace, LogDefault, "Destroy render surface");
    if(frame && frame->renderSurface && frame->renderSurface->GetVulkanSurface())
        delete frame->renderSurface->GetVulkanSurface();

    Log(Trace, LogDefault, "Shutdown vulkan render engine");
    if (vulkanRenderEngine)
    {
        vulkanRenderEngine->Shutdown();
        delete vulkanRenderEngine;
    }
}