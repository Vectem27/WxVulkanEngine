#ifndef VULKANDESCRIPTORUTILS_H
#define VULKANDESCRIPTORUTILS_H

#include <vulkan/vulkan.h>

class VulkanDescriptorUtils
{
public:
    static VkResult AllocateSet(const VkDescriptorPool& pool, const VkDescriptorSetLayout& setLayouts, VkDescriptorSet& descSet)
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &setLayouts;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorPool = pool;

        return vkAllocateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(),&allocInfo, &descSet);
    }
};

#endif // VULKANDESCRIPTORUTILS_H
