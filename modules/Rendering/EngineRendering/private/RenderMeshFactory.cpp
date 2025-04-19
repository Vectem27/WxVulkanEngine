#include "RenderMeshFactory.h"

#include "Logger.h"

using namespace Rendering;

std::string RenderMeshFactory::renderAPI{""};

RHI::IRenderMesh *CreateVulkanMesh(Vertex *vertices, uint32_t vertexCount, RenderMeshFactory::MeshPart *meshParts, uint32_t meshPartCount)
{
    Log(Error, "LogRendering", "Vulkan render mesh creation not implemented yet");
    return nullptr;
}

RHI::IRenderMesh *Rendering::RenderMeshFactory::CreateRenderMesh(Vertex *vertices, uint32_t vertexCount, MeshPart *meshParts, uint32_t meshPartCount)
{
    if (GetRenderAPI() == "Vulkan")
    {
        return CreateVulkanMesh(vertices, vertexCount, meshParts, meshPartCount);
    }

    if(GetRenderAPI() == "")
        Log(Warning, "LogRendering", "Render mesh factory : render API not defined");
    else
        Log(Warning, "LogRendering", "Render mesh factory : render API not supported : %s", GetRenderAPI().c_str());
    
    return nullptr;
}
