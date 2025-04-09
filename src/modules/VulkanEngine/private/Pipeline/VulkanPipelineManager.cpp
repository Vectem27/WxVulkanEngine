#include "Pipeline/VulkanPipelineManager.h"
#include <stdexcept>
#include <vector>
#include <array>

VulkanPipelineManager::VulkanPipelineManager(VkDevice device)
{
    InitSamplers(device);
    InitDescriptorSetLayouts(device);
    InitPipelineLayouts(device);
    InitLightingPipelineLayouts(device);   
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

    // Binding pour la matrice lumière (Set 2, Binding 2)
    VkDescriptorSetLayoutBinding lightFrangmentBufferBinding{};
    lightFrangmentBufferBinding.binding = 1;
    lightFrangmentBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    lightFrangmentBufferBinding.descriptorCount = 1;
    lightFrangmentBufferBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // Utilisé dans le fragment shader
    lightFrangmentBufferBinding.pImmutableSamplers = nullptr;

    std::vector<VkDescriptorSetLayoutBinding> shadowBindings = {
        shadowMapBinding, lightFrangmentBufferBinding
    };

    layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = shadowBindings.size();
    layoutInfo.pBindings = shadowBindings.data();
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &shadowMapDescriptorLayout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create shadow map descriptor set layout!");



    std::array<VkDescriptorSetLayoutBinding, 3> bindings = {};

    // Position (World Space)
    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    // Normals
    bindings[1].binding = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    // Albedo + Specular
    bindings[2].binding = 2;
    bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[2].descriptorCount = 1;
    bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &gBufferDescriptorLayout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create shadow map descriptor set layout!");


    std::array<VkDescriptorSetLayoutBinding, 2> lightingBindings = {};
    // Lights UBO
    lightingBindings[0].binding = 0;
    lightingBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    lightingBindings[0].descriptorCount = 1;
    lightingBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    // Shadow Map (optionnel)
    lightingBindings[1].binding = 1;
    lightingBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    lightingBindings[1].descriptorCount = 1;
    lightingBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(lightingBindings.size());
    layoutInfo.pBindings = lightingBindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &lightingDescriptorLayout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create shadow map descriptor set layout!");

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

void VulkanPipelineManager::InitLightingPipelineLayouts(VkDevice device)
{
    std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts = {
        gBufferDescriptorLayout,
        lightingDescriptorLayout
    };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &lightingPipelineLayout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create lighting pipeline layout !");
}