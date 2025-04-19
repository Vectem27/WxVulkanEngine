#ifndef SURFACERENDERMATERIALFACTORY_H
#define SURFACERENDERMATERIALFACTORY_H

#include <string>
#include "IRenderMaterial.h"

namespace Rendering 
{

    class SurfaceRenderMaterialFactory 
    {
    public:
        struct ShaderInfo
        {
            /**
             * @brief Shader byte code
             */
            void* shaderCode{nullptr};

            /**
             * @brief Shader byte code size in bytes
             */
            size_t shaderCodeSize{0};
        };

        struct PipelineInfo
        {
            ShaderInfo vertexShaderInfo;

            ShaderInfo tessControlShaderInfo;
            ShaderInfo tessEvalShaderInfo;

            ShaderInfo geometryShaderInfo;

            ShaderInfo fragmentShaderInfo;
        };
    private:
        SurfaceRenderMaterialFactory() = delete;
        ~SurfaceRenderMaterialFactory() = delete;
    public:
        static void SetRenderAPI(std::string renderAPI) { SurfaceRenderMaterialFactory::renderAPI = renderAPI; }
        static std::string GetRenderAPI() { return renderAPI; }

        static RHI::IRenderMaterial* CreateSurfaceRenderMaterial(
            const PipelineInfo& renderPiplineInfo, 
            const PipelineInfo& shadowMapPipelineInfo
        );

    private:
        static std::string renderAPI;
    };

} // namespace Rendering

#endif // SURFACERENDERMATERIALFACTORY_H