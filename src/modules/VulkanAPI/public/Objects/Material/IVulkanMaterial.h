#ifndef IVULKANMATERIAL_H
#define IVULKANMATERIAL_H

#include <vulkan/vulkan.h>
#include <string>

struct MaterialInfo
{
    std::string vertexShader = "";
    std::string fragmentShader = "";
    std::string shadowMapVertexShader = "";

    std::string lightingVertexShader = "";
    std::string lightingFragmentShader = "";
};



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

class IVulkanMaterial
{
public:
    // TODO: Remove these getters
    virtual VkPipeline GetBasePipeline() const = 0;
    virtual VkPipeline GetShadowMapPipeline() const = 0;

    virtual void Bind(VkCommandBuffer commandBuffer) const = 0;
};

#endif // IVULKANMATERIAL_H