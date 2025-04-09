#ifndef VULKANPIPELINEMANAGER_H
#define VULKANPIPELINEMANAGER_H

#include <vulkan/vulkan.h>

class VulkanPipelineManager
{
public:
    VulkanPipelineManager(VkDevice device);

public:
    const VkSampler GetShadowMapSampler() const { return shadowMapSampler; }

public: // DescriptionSetLayouts Getters
    const VkDescriptorSetLayout& GetCameraDescriptorSetLayout() const { return cameraDescriptorLayout; }
    const VkDescriptorSetLayout& GetObjectDescriptorSetLayout() const { return objectDescriptorLayout; }
    const VkDescriptorSetLayout& GetShadowMapDescriptorSetLayout() const { return shadowMapDescriptorLayout; }
    const VkDescriptorSetLayout& GetLightDescriptorSetLayout() const { return shadowMapDescriptorLayout; }
public: // PipelineLayouts Getters
    VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }
    VkPipelineLayout GetLightingPipelineLayout() const { return lightingPipelineLayout; }

private:
    VkSampler shadowMapSampler;

private: // DescriptionSetLayouts
    VkDescriptorSetLayout cameraDescriptorLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout objectDescriptorLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout shadowMapDescriptorLayout{VK_NULL_HANDLE};

    VkDescriptorSetLayout gBufferDescriptorLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout lightingDescriptorLayout{VK_NULL_HANDLE};
private: // PipelineLayouts
    VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
    VkPipelineLayout lightingPipelineLayout{VK_NULL_HANDLE};

private:
    void InitSamplers(VkDevice device);
    void InitDescriptorSetLayouts(VkDevice device);
    void InitPipelineLayouts(VkDevice device);
    void InitLightingPipelineLayouts(VkDevice device);
};

#endif // VULKANPIPELINEMANAGER_H