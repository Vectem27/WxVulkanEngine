#ifndef RENDERMESHFACTORY_H
#define RENDERMESHFACTORY_H

#include <string>

#include "IRenderMesh.h"
#include "EngineCore.hpp"

namespace Rendering 
{
    class RenderMeshFactory 
    {
    public:
        struct MeshPart
        {
            uint32_t* indicies{nullptr};
            uint32_t indexCount{0};
        };
    private:
        RenderMeshFactory() = delete;
        ~RenderMeshFactory() = delete;
    public:
        static void SetRenderAPI(std::string renderAPI) { RenderMeshFactory::renderAPI = renderAPI; }
        static std::string GetRenderAPI() { return renderAPI; }

        /**
         * @brief Create a render mesh with the given vertices and mesh parts.
         * @note The vertices and mesh parts are copied to the render mesh buffer and can be deleted after this call.
         * 
         * @param vertices The vertices of the mesh.
         * @param vertexCount The number of vertices.
         * @param meshParts The mesh parts of the mesh. One part is one sub mesh with one material.
         * @param meshPartCount The number of mesh parts.
         * @return The created render mesh or nullptr if it failed.
         */
        static RHI::IRenderMesh* CreateRenderMesh(Vertex* vertices, uint32_t vertexCount, MeshPart* meshParts, uint32_t meshPartCount);

    private:
        static std::string renderAPI;
    };

} // namespace Rendering

#endif // RENDERMESHFACTORY_H