#ifndef MATERIAL_H
#define MATERIAL_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <stdexcept>

struct MaterialInfo
{
    std::string vertexShader = "";
    std::string fragmentShader = "";
};


class Material
{
public:
    void Init(class VulkanRenderer* renderer, const MaterialInfo& Info);

    VkPipeline GetGraphicsPipeline() { return graphicsPipeline; }
    VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
    VkDescriptorSet* GetObjectDescriptorSet() { return &objectDescriptorSet; }

    bool Bind(class VulkanCamera* camera);

private:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSet objectDescriptorSet;

    class VulkanRenderer* renderer;

    void allocateDescriptorSet();
private:
    void initShaderModule(VkDevice device, VkShaderModule *shaderModule, const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(device, &createInfo, nullptr, shaderModule) != VK_SUCCESS) 
        {
            throw std::runtime_error("Échec de la création du module shader !");
        }
    }
   
};

#endif // MATERIAL_H