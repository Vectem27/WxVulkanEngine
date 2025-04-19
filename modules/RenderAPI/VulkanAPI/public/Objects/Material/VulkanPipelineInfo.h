#ifndef VULKANPIPELINEINFO_H
#define VULKANPIPELINEINFO_H

#include <vulkan/vulkan.h>

struct VulkanShaderInfo
{
    /**
     * @brief Shader byte code
     */
    void* shaderCode{nullptr};

    /**
     * @brief Shader byte code size in bytes
     */
    size_t shaderCodeSize{0};

    /**
     * @brief Shader module create flags
     * @details This is used to specify additional options when creating the shader module.
     */
    VkShaderModuleCreateFlags flags = 0;
};

struct VulkanPipelineInfo
{
    VulkanShaderInfo vertexShaderInfo;

    VulkanShaderInfo tessControlShaderInfo;
    VulkanShaderInfo tessEvalShaderInfo;

    VulkanShaderInfo geometryShaderInfo;

    VulkanShaderInfo fragmentShaderInfo;
};

#endif // VULKANPIPELINEINFO_H