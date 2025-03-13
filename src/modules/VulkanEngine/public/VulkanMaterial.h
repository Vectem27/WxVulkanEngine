#ifndef VULKANMATERIAL_H
#define VULKANMATERIAL_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <stdexcept>

struct MaterialInfo
{
    std::string vertexShader = "";
    std::string fragmentShader = "";
};


class VulkanMaterial
{
public:
    void Init(class VulkanRenderEngine* renderer, const MaterialInfo& Info);

    VkPipeline GetGraphicsPipeline() { return graphicsPipeline; }
    VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
    VkDescriptorSet* GetObjectDescriptorSet() { return &objectDescriptorSet; }

    bool Bind(class VulkanCamera* camera);

private:
    VkPipeline graphicsPipeline{VK_NULL_HANDLE};
    VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
    VkDescriptorSet objectDescriptorSet{VK_NULL_HANDLE};

    class VulkanRenderEngine* renderer;

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

#endif // VULKANMATERIAL_H