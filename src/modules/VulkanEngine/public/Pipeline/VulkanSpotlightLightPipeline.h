#ifndef VULKANSPOTLIGHTLIGHTPIPELINE_H
#define VULKANSPOTLIGHTLIGHTPIPELINE_H

#include <vulkan/vulkan.h>
#include <string>

class VulkanPipelineManager;

class VulkanSpotlightLightPipeline
{
public:
    void InitPipeline(VkDevice device, VulkanPipelineManager* pipelineManager, std::string vertexShaderFile, std::string fragmentShaderFile);
    void Bind(VkCommandBuffer commandBuffer) const;

private:
    VkPipeline GetPipeline() const { return pipeline; }
    VkPipeline pipeline;
};

#endif // VULKANSPOTLIGHTLIGHTPIPELINE_H