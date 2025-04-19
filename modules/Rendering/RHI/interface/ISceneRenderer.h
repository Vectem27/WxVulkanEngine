#ifndef ISCENERENDERER_H
#define ISCENERENDERER_H



namespace RHI
{
    class IRenderScene;
    class IRenderCamera;
    class IRenderLight;

    class ISceneRenderer 
    {
    public:
        virtual ~ISceneRenderer() = default;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        virtual void RenderScene(const IRenderScene& scene, const IRenderCamera& camera) = 0;
        virtual void RenderSceneShadowMap(const IRenderScene& scene, const IRenderCamera& camera) = 0;
    };
}

#endif // ISCENERENDERER_H