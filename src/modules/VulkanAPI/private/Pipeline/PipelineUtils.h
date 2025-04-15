#ifndef PIPELINEUTILS_H
#define PIPELINEUTILS_H

#include <vector>
#include <iostream>
#include <fstream>
#include <vulkan/vulkan.h>
#include "VulkanDeviceManager.h"

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

inline void InitShaderModule(VkShaderModule *shaderModule, const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(GetVulkanDeviceManager().GetDeviceChecked(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) 
    {
        throw std::runtime_error("Échec de la création du module shader !");
    }
}

#endif // PIPELINEUTILS_H