#include "VulkanPipelineUtils.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Logger.h"

#include "VulkanDeviceManager.h"

using namespace VulkanPipelineUtils;

VkShaderModule VulkanPipelineUtils::CreateShaderModule(const VulkanShaderInfo& shaderInfo) 
{
    VkShaderModule shaderModule = VK_NULL_HANDLE;

    if (shaderInfo.shaderCode == nullptr || shaderInfo.shaderCodeSize <= 0) 
    {
        return VK_NULL_HANDLE;
    }

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderInfo.shaderCodeSize;
    createInfo.flags = shaderInfo.flags;
    createInfo.pCode = reinterpret_cast<uint32_t*>(shaderInfo.shaderCode);
    createInfo.pNext = nullptr;

    VkResult result = vkCreateShaderModule(GetVulkanDeviceManager().GetDeviceChecked(), &createInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS) 
    {
        Log(Error, "Vulkan", "Failed to create shader module. Result code : %d", result);
        throw std::runtime_error("Failed to create shader module");
    }
    
    return shaderModule;
}

VulkanShaderStages VulkanPipelineUtils::CreateShaderStages(const VulkanPipelineInfo& pipelineInfo) 
{
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    if (auto vertexShaderModule = CreateShaderModule(pipelineInfo.vertexShaderInfo); vertexShaderModule != VK_NULL_HANDLE)
    {
        VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
        vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageInfo.module = vertexShaderModule;
        vertexShaderStageInfo.pName = "main";

        shaderStages.push_back(vertexShaderStageInfo);
    }

    if (auto tessControlShaderModule = CreateShaderModule(pipelineInfo.tessControlShaderInfo); tessControlShaderModule != VK_NULL_HANDLE)
    {
        VkPipelineShaderStageCreateInfo tessControlShaderStageInfo = {};
        tessControlShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        tessControlShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        tessControlShaderStageInfo.module = tessControlShaderModule;
        tessControlShaderStageInfo.pName = "main";

        shaderStages.push_back(tessControlShaderStageInfo);
    }

    if (auto tessEvalShaderModule = CreateShaderModule(pipelineInfo.tessEvalShaderInfo); tessEvalShaderModule != VK_NULL_HANDLE)
    {
        VkPipelineShaderStageCreateInfo tessEvalShaderStageInfo = {};
        tessEvalShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        tessEvalShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        tessEvalShaderStageInfo.module = tessEvalShaderModule;
        tessEvalShaderStageInfo.pName = "main";

        shaderStages.push_back(tessEvalShaderStageInfo);
    }

    if (auto geometryShaderModule = CreateShaderModule(pipelineInfo.geometryShaderInfo); geometryShaderModule != VK_NULL_HANDLE)
    {
        VkPipelineShaderStageCreateInfo geometryShaderStageInfo = {};
        geometryShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        geometryShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
        geometryShaderStageInfo.module = geometryShaderModule;
        geometryShaderStageInfo.pName = "main";

        shaderStages.push_back(geometryShaderStageInfo);
    }

    if (auto fragmentShaderModule = CreateShaderModule(pipelineInfo.fragmentShaderInfo); fragmentShaderModule != VK_NULL_HANDLE)
    {
        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
        fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageInfo.module = fragmentShaderModule;
        fragmentShaderStageInfo.pName = "main";

        shaderStages.push_back(fragmentShaderStageInfo);
    }        

    VulkanShaderStages result{};
    result.shaderStageCount = static_cast<uint8_t>(shaderStages.size());
    result.shaderStageInfos = (VkPipelineShaderStageCreateInfo*)malloc(sizeof(VkPipelineShaderStageCreateInfo) * result.shaderStageCount);  

    std::copy(shaderStages.begin(), shaderStages.end(), result.shaderStageInfos);

    if (result.shaderStageCount == 0)
    {
        Log(Warning, "Vulkan", "No shader stage created");
    }

    return result;
}

void VulkanPipelineUtils::DestroyShaderStages(VulkanShaderStages& shaderStageResult) 
{
    if (shaderStageResult.shaderStageInfos != nullptr) 
    {
        for (uint8_t i = 0; i < shaderStageResult.shaderStageCount; ++i) 
        {
            vkDestroyShaderModule(GetVulkanDeviceManager().GetDeviceChecked(), shaderStageResult.shaderStageInfos[i].module, nullptr);
        }

        free(shaderStageResult.shaderStageInfos);
        shaderStageResult.shaderStageInfos = nullptr;
    }
}