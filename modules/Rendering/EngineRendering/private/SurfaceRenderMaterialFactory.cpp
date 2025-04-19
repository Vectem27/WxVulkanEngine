#include "SurfaceRenderMaterialFactory.h"

// EngineCore
#include "Logger.h"

// VulkanAPI
#include "VulkanPipelineInfo.h"

// VulkanRendering
#include "VulkanSurfaceRenderMaterial.h"

std::string Rendering::SurfaceRenderMaterialFactory::renderAPI{""};

RHI::IRenderMaterial* CreateVulkanSurfaceMaterial(
    const Rendering::SurfaceRenderMaterialFactory::PipelineInfo &renderPiplineInfo, 
    const Rendering::SurfaceRenderMaterialFactory::PipelineInfo &shadowMapPipelineInfo)
{
    Vulkan::SurfaceRenderMaterial *material = new Vulkan::SurfaceRenderMaterial();

    VulkanPipelineInfo pipelineInfo;
    pipelineInfo.vertexShaderInfo.shaderCode = renderPiplineInfo.vertexShaderInfo.shaderCode;
    pipelineInfo.vertexShaderInfo.shaderCodeSize = renderPiplineInfo.vertexShaderInfo.shaderCodeSize;
    pipelineInfo.fragmentShaderInfo.shaderCode = renderPiplineInfo.fragmentShaderInfo.shaderCode;
    pipelineInfo.fragmentShaderInfo.shaderCodeSize = renderPiplineInfo.fragmentShaderInfo.shaderCodeSize;
    pipelineInfo.tessControlShaderInfo.shaderCode = renderPiplineInfo.tessControlShaderInfo.shaderCode;
    pipelineInfo.tessControlShaderInfo.shaderCodeSize = renderPiplineInfo.tessControlShaderInfo.shaderCodeSize;
    pipelineInfo.tessEvalShaderInfo.shaderCode = renderPiplineInfo.tessEvalShaderInfo.shaderCode;
    pipelineInfo.tessEvalShaderInfo.shaderCodeSize = renderPiplineInfo.tessEvalShaderInfo.shaderCodeSize;
    pipelineInfo.geometryShaderInfo.shaderCode = renderPiplineInfo.geometryShaderInfo.shaderCode;
    pipelineInfo.geometryShaderInfo.shaderCodeSize = renderPiplineInfo.geometryShaderInfo.shaderCodeSize;
    
    material->InitRenderPipeline(pipelineInfo);
    // TODO: Init shadow map pipeline
    //material->InitShadowMapPipeline(shadowMapPipelineInfo);
    return material;
}

RHI::IRenderMaterial *Rendering::SurfaceRenderMaterialFactory::CreateSurfaceRenderMaterial(const PipelineInfo &renderPiplineInfo, const PipelineInfo &shadowMapPipelineInfo)
{
    if (GetRenderAPI() == "Vulkan")
    {
        return CreateVulkanSurfaceMaterial(renderPiplineInfo, shadowMapPipelineInfo);
    }

    if (GetRenderAPI() == "")
        Log(Warning, "LogRendering", "Surface render material factory : render API not defined");
    else
        Log(Warning, "LogRendering", "Surface render material factory : render API not supported : %s", GetRenderAPI().c_str());
    return nullptr;
}