#include "RenderMeshFactory.h"

#include "Logger.h"

#include "VulkanRenderMesh.h"

using namespace Rendering;

std::string RenderMeshFactory::renderAPI{""};

RHI::IRenderMesh *CreateVulkanMesh(Vertex *vertices, uint32_t vertexCount, RenderMeshFactory::MeshPart *meshParts, uint32_t meshPartCount)
{
    Vulkan::RenderMesh *mesh = new Vulkan::RenderMesh();
    mesh->SetVertices(vertices, vertexCount);

    for (uint32_t i = 0; i < meshPartCount; ++i)
    {
        mesh->AddMeshPart(meshParts[i].indicies, meshParts[i].indexCount);
    }

    return mesh;
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
