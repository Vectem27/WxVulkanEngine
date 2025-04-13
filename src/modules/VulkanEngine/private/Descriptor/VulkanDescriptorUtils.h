#ifndef VULKANDESCRIPTORUTILS_H
#define VULKANDESCRIPTORUTILS_H

#include <vulkan/vulkan.h>

namespace VulkanDescriptorUtils
{
    /**
     * @brief Create and allocate a descriptor set
     * @param pool The descriptor pool which allocate the descriptor set
     * @param setLayouts The descriptor set layout
     * 
     * @note Parameters are checked and can throw a runtime error
     */
    VkDescriptorSet AllocateSet(
        VkDescriptorPool pool, 
        VkDescriptorSetLayout setLayouts
    );
};

#endif // VULKANDESCRIPTORUTILS_H
