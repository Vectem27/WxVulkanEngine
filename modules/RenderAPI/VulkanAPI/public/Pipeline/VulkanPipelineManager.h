#ifndef VULKANPIPELINEMANAGER_H
#define VULKANPIPELINEMANAGER_H

#include <vulkan/vulkan.h>

class VulkanPipelineManager
{
    VulkanPipelineManager();

public:
    static VulkanPipelineManager& GetInstance()
    {
        static VulkanPipelineManager instance;
        return instance;
    }

public:
    const VkSampler GetShadowMapSampler() const { return shadowMapSampler; }
    const VkSampler GetGBufferSampler() const { return gBufferSampler; }

    uint32_t GetMaxLightsPerDescriptor() const { return 16; }
public: // DescriptionSetLayouts Getters
    const VkDescriptorSetLayout& GetCameraDescriptorSetLayout() const { return cameraDescriptorLayout; }
    const VkDescriptorSetLayout& GetObjectDescriptorSetLayout() const { return objectDescriptorLayout; }
    const VkDescriptorSetLayout& GetPostprocessDescriptorSetLayout() const { return postprocessDescriptorLayout; }
    const VkDescriptorSetLayout& GetLightDescriptorSetLayout() const { return lightingDescriptorLayout; }
    const VkDescriptorSetLayout& GetgBufferDescriptorSetLayout() const { return gBufferDescriptorLayout; }
    
public: // PipelineLayouts Getters
    VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }
    VkPipelineLayout GetLightingPipelineLayout() const { return lightingPipelineLayout; }
    VkPipelineLayout GetPostprocessPipelineLayout() const { return postprocessPipelineLayout; }

private:
    VkSampler shadowMapSampler;
    VkSampler gBufferSampler;

private: // DescriptionSetLayouts
    VkDescriptorSetLayout cameraDescriptorLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout objectDescriptorLayout{VK_NULL_HANDLE};

    VkDescriptorSetLayout gBufferDescriptorLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout lightingDescriptorLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout postprocessDescriptorLayout{VK_NULL_HANDLE};
private: // PipelineLayouts
    VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
    VkPipelineLayout lightingPipelineLayout{VK_NULL_HANDLE};
    VkPipelineLayout postprocessPipelineLayout{VK_NULL_HANDLE};

private:
    void InitSamplers(VkDevice device);
    void InitDescriptorSetLayouts(VkDevice device);
    void InitPipelineLayouts(VkDevice device);
    void InitLightingPipelineLayouts(VkDevice device);
    void InitPostprocessPipelineLayouts(VkDevice device);
};

VulkanPipelineManager& GetVulkanPipelineManager();

#endif // VULKANPIPELINEMANAGER_H