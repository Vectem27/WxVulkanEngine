#include "Pipeline/VulkanPipelineManager.h"
#include <stdexcept>

VulkanPipelineManager::VulkanPipelineManager(VkDevice device)
{
    InitDescriptorSetLayouts(device);
    InitPipelineLayouts(device);   
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
    layoutInfo.pBindings = &cameraVpBinding;
    
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &objectDescriptorLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create object descriptor set layout !");
    }
}

void VulkanPipelineManager::InitPipelineLayouts(VkDevice device)
{
    VkDescriptorSetLayout descSetLayouts[] 
    {
        cameraDescriptorLayout,
        objectDescriptorLayout
    };

    // Création du pipeline layout (aucun uniform ou push constant ici)
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 2;
    pipelineLayoutInfo.pSetLayouts = descSetLayouts; // Ajouté

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout !");
    }
}