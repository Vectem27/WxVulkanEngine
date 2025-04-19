#ifndef PIPELINEUTILS_H
#define PIPELINEUTILS_H

#include <vulkan/vulkan.h>

#include "VulkanPipelineInfo.h"

// TODO: Remove these includes after deleting the deprecated functions
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "VulkanDeviceManager.h"

//TODO: Move this function in another module, VulkanAPI is not the right place for it
[[deprecated]]
inline std::vector<char> ReadFile(const std::string &filename)
{
    try
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file)
        {
            std::cerr << "Erreur lors de l'ouverture du fichier : " << filename << std::endl;
            return {};
        }

        std::streamsize size = file.tellg();
        if (size <= 0)
        {
            std::cerr << "Fichier vide ou erreur de lecture : " << filename << std::endl;
            return {};
        }

        std::vector<char> buffer(size);
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), size);

        return buffer;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception : " << e.what() << '\n';
        return {};
    }
}

[[deprecated]]
inline void InitShaderModule(VkShaderModule *shaderModule, const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    createInfo.flags = 0;
    if (vkCreateShaderModule(GetVulkanDeviceManager().GetDeviceChecked(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) 
    {
        throw std::runtime_error("Échec de la création du module shader !");
    }
}


/**
 * @namespace VulkanPipelineUtils
 * @brief Namespace for Vulkan pipeline utilities
 * 
 * @author Vectem
 */
namespace VulkanPipelineUtils
{
    /**
     * @brief Structure to hold shader stages create info
     * @details This structure holds the count and the array of shader stage create info structures.
     */
    struct VulkanShaderStages
    {
        uint8_t shaderStageCount{0};
        VkPipelineShaderStageCreateInfo* shaderStageInfos{nullptr};
    };

    /**
     * @brief Create a Shader Module object
     * 
     * @param shaderInfo The shader info structure containing the shader code and its size
     * @return The created shader module 
     */
    VkShaderModule CreateShaderModule(const VulkanShaderInfo& shaderInfo);

    /**
     * @brief Create shader stages from the given pipeline info
     * 
     * @param pipelineInfo The pipeline info structure containing the shader info for each stage
     * @return The created shader stages
     */
    VulkanShaderStages CreateShaderStages(const VulkanPipelineInfo& pipelineInfo);

    /**
     * @brief Destroy shader stages and free the memory allocated for them
     * 
     * @param shaderStageResult The shader stages to destroy
     */
    void DestroyShaderStages(VulkanShaderStages& shaderStageResult);
}


#endif // PIPELINEUTILS_H