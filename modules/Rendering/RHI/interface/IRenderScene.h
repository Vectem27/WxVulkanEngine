#ifndef IRENDERSCENE_H
#define IRENDERSCENE_H

#include <inttypes.h>

namespace RHI
{
    class IRenderable;
    class IRenderLight;

    class IRenderScene 
    {
    public:
        /**
         * @brief Get all meshes to render in the scene
         * 
         * @param meshes Output array of meshes to render
         * @param meshCount Output number of meshes in the array
         */
        virtual void GetRenderMeshes(IRenderable*& meshes, uint32_t& meshCount) const = 0;

        /**
         * @brief Get all lights to render in the scene
         * 
         * @param lights Output array of lights to render
         * @param lightCount Output number of lights in the array
         */
        virtual void GetRenderLights(IRenderLight*& lights, uint32_t& lightCount) const = 0;
    };
}

#endif // IRENDERSCENE_H