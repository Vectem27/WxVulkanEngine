#ifndef VULKANDESCRIPTORPOOLMANAGER_H
#define VULKANDESCRIPTORPOOLMANAGER_H

#include <vulkan/vulkan.h>
#include <vector>

class VulkanDescriptorPoolManager
{
public:
    VulkanDescriptorPoolManager(class VulkanRenderEngine* renderEngine);
    ~VulkanDescriptorPoolManager();

public:
    bool AllocateDescriptorSets(const VkDescriptorSetLayout* setLayouts, uint32_t descriptorSetCount, VkDescriptorSet *descriptorSets);

private:
    VkDescriptorPool CreateNewDescriptorPool();
private:
    class VulkanRenderEngine* renderEngine;
    std::vector<VkDescriptorPool> descriptorPools;
};

#endif // VULKANDESCRIPTORPOOLMANAGER_H