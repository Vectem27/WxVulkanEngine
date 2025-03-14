#ifndef VULKANPIPELINEMANAGER_H
#define VULKANPIPELINEMANAGER_H

#include <vulkan/vulkan.h>

class VulkanPipelineManager
{
public:
    VulkanPipelineManager(VkDevice device);

public: // DescriptionSetLayouts Getters
    const VkDescriptorSetLayout& GetCameraDescriptorSetLayout() const { return cameraDescriptorLayout; }
    const VkDescriptorSetLayout& GetObjectDescriptorSetLayout() const { return objectDescriptorLayout; }

public: // PipelineLayouts Getters
    VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }

private: // DescriptionSetLayouts
    VkDescriptorSetLayout cameraDescriptorLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout objectDescriptorLayout{VK_NULL_HANDLE};

private: // PipelineLayouts
    VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};

private:
    void InitDescriptorSetLayouts(VkDevice device);
    void InitPipelineLayouts(VkDevice device);
};

#endif // VULKANPIPELINEMANAGER_H