#include "Pipeline/VulkanPipelineManager.h"
#include <stdexcept>
#include <vector>

VulkanPipelineManager::VulkanPipelineManager(VkDevice device)
{
    InitSamplers(device);
    InitDescriptorSetLayouts(device);
    InitPipelineLayouts(device);   
}
void VulkanPipelineManager::InitSamplers(VkDevice device)
{
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerInfo.compareEnable = VK_TRUE;
    samplerInfo.compareOp = VK_COMPARE_OP_LESS;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0f;

    vkCreateSampler(device, &samplerInfo, nullptr, &shadowMapSampler);
}

void VulkanPipelineManager::InitDescriptorSetLayouts(VkDevice device)
{
    // Layout pour camera_vp (Set 0, Binding 0)
    VkDescriptorSetLayoutBinding cameraVpBinding{};
    cameraVpBinding.binding = 0;
    cameraVpBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    cameraVpBinding.descriptorCount = 1;
    cameraVpBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    cameraVpBinding.pImmutableSamplers = nullptr;


    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &cameraVpBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &cameraDescriptorLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create camera descriptor set layout !");
    }


    // Layout pour object (Set 1, Binding 0)
    VkDescriptorSetLayoutBinding objectBinding{};
    objectBinding.binding = 0;
    objectBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    objectBinding.descriptorCount = 1;
    objectBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    objectBinding.pImmutableSamplers = nullptr;

    layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &objectBinding;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &objectDescriptorLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create object descriptor set layout !");
    }

    // Layout pour shadow map (Set 2, Binding 0)
    VkDescriptorSetLayoutBinding shadowMapBinding{};
    shadowMapBinding.binding = 0;
    shadowMapBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    shadowMapBinding.descriptorCount = 1;
    shadowMapBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // Utilisé dans le fragment shader
    shadowMapBinding.pImmutableSamplers = nullptr;

    // Binding pour la matrice lumière (Set 2, Binding 1)
    VkDescriptorSetLayoutBinding lightVertexBufferBinding{};
    lightVertexBufferBinding.binding = 1;
    lightVertexBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    lightVertexBufferBinding.descriptorCount = 1;
    lightVertexBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Utilisé dans le vertex shader
    lightVertexBufferBinding.pImmutableSamplers = nullptr;

    // Binding pour la matrice lumière (Set 2, Binding 2)
    VkDescriptorSetLayoutBinding lightFrangmentBufferBinding{};
    lightFrangmentBufferBinding.binding = 2;
    lightFrangmentBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    lightFrangmentBufferBinding.descriptorCount = 1;
    lightFrangmentBufferBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // Utilisé dans le fragment shader
    lightFrangmentBufferBinding.pImmutableSamplers = nullptr;

    std::vector<VkDescriptorSetLayoutBinding> shadowBindings = {
        shadowMapBinding, lightVertexBufferBinding, lightFrangmentBufferBinding
    };

    layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = shadowBindings.size();
    layoutInfo.pBindings = shadowBindings.data();
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &shadowMapDescriptorLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shadow map descriptor set layout!");
    }
}

void VulkanPipelineManager::InitPipelineLayouts(VkDevice device)
{
    std::vector<VkDescriptorSetLayout> descSetLayouts 
    {
        cameraDescriptorLayout,   // Set 0: Camera VP
        objectDescriptorLayout,   // Set 1: Object transforms
        shadowMapDescriptorLayout // Set 2: Shadow map + light matrix
    };

    // Création du pipeline layout (aucun uniform ou push constant ici)
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = descSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = descSetLayouts.data(); // Ajouté

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout !");
    }
}