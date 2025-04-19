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

#include "VulkanDeviceManager.h"
#include "VulkanRenderPassManager.h"

#include "VulkanSceneRenderer.h"

#include "Engine.h"
#include "SwapchainTarget.h"

#include <fstream>

#include "RenderMeshFactory.h"
#include "SurfaceRenderMaterialFactory.h"

//TODO: Delete and move this function : 
inline std::vector<char> ReadFile(const std::string &filename)
{
    try
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file)
        {
            std::cerr << "Erreur lors de l'ouverture du fichier : " << filename << std::endl;
            return {};
        }

        std::streamsize size = file.tellg();
        if (size <= 0)
        {
            std::cerr << "Fichier vide ou erreur de lecture : " << filename << std::endl;
            return {};
        }

        std::vector<char> buffer(size);
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), size);

        return buffer;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception : " << e.what() << '\n';
        return {};
    }
}

// TODO: Delete this function
RHI::IRenderMesh* CreateRenderCubeTest()
{
    std::vector<Vertex> vertices = 
    {
        // Z+
        {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},  // Vertex 1 (red)
        {{-0.5f,  0.5f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},  // Vertex 3 (blue)
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},  // Vertex 2 (green)
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},  // Vertex 4 (yellow)

        // Z-
        {{-0.5f, -0.5f,  0.5f}, {0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // Vertex 5 (purple)
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // Vertex 6 (cyan)
        {{-0.5f,  0.5f,  0.5f}, {0.5f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // Vertex 7 (gray)
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},  // Vertex 8 (white)

        // X-
        {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 9
        {{-0.5f, -0.5f,  0.5f}, {0.5f, 0.5f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // Vertex 10
        {{-0.5f,  0.5f, -0.5f}, {0.5f, 1.0f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},  // Vertex 11
        {{-0.5f,  0.5f,  0.5f}, {0.5f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Vertex 12

        // X+
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 13
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},  // Vertex 15
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // Vertex 14
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Vertex 16

        // Y+
        {{-0.5f,  0.5f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 17
        {{-0.5f,  0.5f,  0.5f}, {0.5f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},  // Vertex 19
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // Vertex 18
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // Vertex 20

        // Y-
        {{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},  // Vertex 21
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},  // Vertex 22
        {{-0.5f, -0.5f,  0.5f}, {0.5f, 0.5f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},  // Vertex 23
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},  // Vertex 24
    };

    std::vector<uint32_t> indices = 
    {
        0,  1,  2,  3,  2,  1,     // Z-
        4,  5,  6,  7,  6,  5,     // Z+
        8,  9,  10, 11, 10, 9,     // X-
        12, 13, 14, 15, 14, 13,    // X+
        16, 17, 18, 19, 18, 17,    // Y+
        20, 21, 22, 23, 22, 21,    // Y-
    };

    Rendering::RenderMeshFactory::MeshPart meshPart = {};
    meshPart.indicies = indices.data();
    meshPart.indexCount = indices.size();

    return Rendering::RenderMeshFactory::CreateRenderMesh(
        vertices.data(), vertices.size(), &meshPart, 1);
}

wxVulkanApp::wxVulkanApp()
    : frame(new wxVulkanFrame("Vulkan avec wxWidgets")), 
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
            cameraActor->SetRelativeRotation(Rotation::FromEulerDegrees(rot.x, rot.y, static_cast<float>(yaw)));
        }
    });

    frame->SetSlider2ChangeCallback([this](int pitch){
        if (cameraActor)
        {
            auto rot = cameraActor->GetRelativeTransform().rotation.ToEulerDegrees();
            cameraActor->SetRelativeRotation(Rotation::FromEulerDegrees(static_cast<float>(pitch), rot.y, rot.z));
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

static RHI::IRenderMesh* testRenderMesh = nullptr;
static RHI::IRenderMaterial* testRenderMaterial = nullptr;

void wxVulkanApp::InitVulkan()
{
    Rendering::RenderMeshFactory::SetRenderAPI("Vulkan");
    Rendering::SurfaceRenderMaterialFactory::SetRenderAPI("Vulkan");

    try
    {
        GetVulkanAPIModule().InitModule();
        
        frame->renderSurface->InitVulkanSurface();
        swapchainTarget = new SwapchainTarget(frame->renderSurface->GetVulkanSurface());

        auto projLight2 = new SpotlightLightComponent();
        projLight = new SpotlightLightComponent();

        camera->VulkanCamera::Init(static_cast<IVulkanRenderTarget*>(swapchainTarget->GetRenderTarget(RenderAPI::Vulkan)));

        cube = new Cube();
        cube->Init();
        tinyCube = new Cube();
        tinyCube->Init();
        floor = new Cube();
        floor->Init();
        
        material = new VulkanOpaqueMaterial();
        
        {
            auto vertShaderCode = ReadFile("shaders/shader.vert");
            auto fragShaderCode = ReadFile("shaders/shader.frag");

            VulkanPipelineInfo meshPipelineInfo = {};
            meshPipelineInfo.vertexShaderInfo.shaderCode = vertShaderCode.data();
            meshPipelineInfo.vertexShaderInfo.shaderCodeSize = vertShaderCode.size();
            meshPipelineInfo.fragmentShaderInfo.shaderCode = fragShaderCode.data();
            meshPipelineInfo.fragmentShaderInfo.shaderCodeSize = fragShaderCode.size();

            material->CreatePipelines(meshPipelineInfo);

            auto shadowMapVertShaderCode = ReadFile("shaders/shadowMap.vert");
            VulkanPipelineInfo meshShadowMapPipelineInfo = {};
            meshShadowMapPipelineInfo.vertexShaderInfo.shaderCode = shadowMapVertShaderCode.data();
            meshShadowMapPipelineInfo.vertexShaderInfo.shaderCodeSize = shadowMapVertShaderCode.size();
            

            material->CreateShadowMapPipeline(meshShadowMapPipelineInfo);

            Rendering::SurfaceRenderMaterialFactory::PipelineInfo pipelineInfo = {};
            pipelineInfo.vertexShaderInfo.shaderCode = vertShaderCode.data();
            pipelineInfo.vertexShaderInfo.shaderCodeSize = vertShaderCode.size();
            pipelineInfo.fragmentShaderInfo.shaderCode = fragShaderCode.data();
            pipelineInfo.fragmentShaderInfo.shaderCodeSize = fragShaderCode.size();

            Rendering::SurfaceRenderMaterialFactory::PipelineInfo shadowMapPipelineInfo = {};
            shadowMapPipelineInfo.vertexShaderInfo.shaderCode = shadowMapVertShaderCode.data();
            shadowMapPipelineInfo.vertexShaderInfo.shaderCodeSize = shadowMapVertShaderCode.size();
            
            testRenderMaterial = Rendering::SurfaceRenderMaterialFactory::CreateSurfaceRenderMaterial(
                pipelineInfo,
                shadowMapPipelineInfo
            );
        }

        testRenderMesh = CreateRenderCubeTest();
        testRenderMesh->AddInstance({{0,0,0}, Rotation::FromEulerDegrees(0,0,0), {1,1,1}});
        testRenderMesh->SetMaterial(0, testRenderMaterial);


        cube->SetMaterial(material);
        tinyCube->SetMaterial(material);
        tinyCube->SetRelativePosition({1.0f,1.0f,1.0f});
        tinyCube->SetRelativeRotation(Rotation::FromEulerDegrees(0,0,45.0f));
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

        camera->SetRelativeTransform(Transform({-5,-2,2}, Rotation::FromEulerDegrees(-10,0,45), {1,1,1}));
        li->SetRelativeTransform(Transform({-6,6,4}, Rotation::FromEulerDegrees(-30,0,-45), {1,1,1}));
        li2->SetRelativeTransform(Transform({-3,3,8}, Rotation::FromEulerDegrees(-80,0,45), {1,1,1}));
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
    cubeActor->SetRelativeRotation(Rotation::FromEulerDegrees(pitch, roll, yaw));
    tinyCube->SetRelativeRotation(Rotation::FromEulerDegrees(0,0,yaw2));        
    try
    {
        sceneRenderer->RenderWorld(world, camera);
        static_cast<VulkanSwapchain*>(swapchainTarget->GetRenderTarget(RenderAPI::Vulkan))->Present();
        
        
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
    delete swapchainTarget;
    swapchainTarget = nullptr;
    
    Log(Trace, LogDefault, "Destroy render surface");
    if(frame && frame->renderSurface && frame->renderSurface->GetVulkanSurface())
        delete frame->renderSurface->GetVulkanSurface();

    Log(Trace, LogDefault, "Shutdown vulkan render engine");
    GetVulkanAPIModule().ShutdownModule();
}