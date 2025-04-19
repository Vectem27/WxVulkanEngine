#ifndef VULKANDESCRIPTORUTILS_H
#define VULKANDESCRIPTORUTILS_H

#include <vulkan/vulkan.h>

class VulkanBuffer;

/**
 * @namespace VulkanDescriptorUtils
 * @brief Contain vulkan descriptor set/pool utility functions
 */
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

    /**
     * @brief Update a descriptor set with a buffer 
     * 
     * @param descriptorSet The descriptor set to update
     * @param buffer The buffer to bind to the descriptor set
     * @param type The type of the descriptor set (uniform buffer, storage buffer, etc.)
     * @param binding The binding index of the descriptor set
     */
    void UpdateSet(
        VkDescriptorSet descriptorSet, const VulkanBuffer& buffer, 
        VkDescriptorType type, uint32_t binding
    ) noexcept;

    //TODO: Add UpdateSetDynamic for dynamic buffers

    /**
     * @brief Free a descriptor set
     * @param descriptorPool The descriptor pool which created the descriptor set
     * @param descriptorSet The descriptor set to free
     * 
     * @note Parameters are checked but don't throw errors
     * @warning This function should be called on a descriptor set created by a pool with the free descriptor set flag
     */
    void FreeDescriptorSet(VkDescriptorPool descriptorPool, VkDescriptorSet& descriptorSet) noexcept;

    /**
     * @brief Destroy a descriptor pool
     * @param commandPool The descriptor pool to destroy
     * 
     * @note Parameters are checked but don't throw errors
     */
    void DestroyDescriptorPool(VkDescriptorPool& descriptorPool) noexcept;
};

#endif // VULKANDESCRIPTORUTILS_H
