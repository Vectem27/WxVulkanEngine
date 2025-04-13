#ifndef VULKANDESCRIPTORPOOLBUILDER_H
#define VULKANDESCRIPTORPOOLBUILDER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <inttypes.h>

/**
 * @brief A vulkan descriptor pool creation helper class
 * @class VulkanDescriptorPoolBuilder
 * 
 * @author Vectem
 */
class VulkanDescriptorPoolBuilder
{
public:
    VulkanDescriptorPoolBuilder() = default;
    ~VulkanDescriptorPoolBuilder() = default;
    VulkanDescriptorPoolBuilder(const VulkanDescriptorPoolBuilder&) = delete;
    VulkanDescriptorPoolBuilder& operator=(const VulkanDescriptorPoolBuilder&) = delete;
    VulkanDescriptorPoolBuilder(VulkanDescriptorPoolBuilder&&) noexcept = delete;
    VulkanDescriptorPoolBuilder& operator=(VulkanDescriptorPoolBuilder&&) noexcept = delete;
public:
    /**
     * @brief Reset the descriptor pool builder for another use.
     */
    VulkanDescriptorPoolBuilder& Reset();

    /**
     * @brief Create the decriptor pool.
     * @return The created descriptor pool
     */
    VkDescriptorPool Build();
    
    /**
     * @brief Add uniform buffer descriptor pool size.
     * @param count The max count of uniform buffers
     */
    VulkanDescriptorPoolBuilder& AddUniformBuffer(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count);}

    /**
     * @brief Add dynamic uniform buffer descriptor pool size.
     * @param count The max count of dynamic uniform buffers
     */
    VulkanDescriptorPoolBuilder& AddUniformBufferDynamic(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, count);}

    /**
     * @brief Add storage buffer descriptor pool size.
     * @param count The max count of storage buffers
     */
    VulkanDescriptorPoolBuilder& AddStorageBuffer(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, count);}

    /**
     * @brief Add dynamic storage buffer descriptor pool size.
     * @param count The max count of dynamic storage buffers
     */
    VulkanDescriptorPoolBuilder& AddStorageBufferDynamic(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, count);}

    /**
     * @brief Add sampled image descriptor pool size.
     * @param count The max count of sampled images
     */
    VulkanDescriptorPoolBuilder& AddSampledImage(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count); }

    /**
     * @brief Add combined image and sampler descriptor pool size.
     * @param count The max count of combined images and samplers
     */
    VulkanDescriptorPoolBuilder& AddCombinedImageSampler(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, count); }

    /**
     * @brief Add sampler descriptor pool size.
     * @param count The max count of samplers
     */
    VulkanDescriptorPoolBuilder& AddSampler(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_SAMPLER, count); }

    /**
     * @brief Add storage image descriptor pool size.
     * @param count The max count of storage images
     */
    VulkanDescriptorPoolBuilder& AddStorageImage(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count); }

    /**
     * @brief Add input attachment descriptor pool size.
     * @param count The max count of input attachments
     */
    VulkanDescriptorPoolBuilder& AddInputAttachment(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, count); }

    /**
     * @brief Add inline uniform block descriptor pool size.
     * @param count The max count of inline uniform blocks
     */
    VulkanDescriptorPoolBuilder& AddInlineUniformBlock(uint32_t count) 
    { return Add(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK, count); }

    /**
     * @brief Add typed descriptor pool size.
     * @param type The type of the descriptor
     * @param count The max count of this type
     */
    VulkanDescriptorPoolBuilder& Add(VkDescriptorType type, uint32_t count);
    
    /**
     * @brief The maximum number of desriptor sets, the pool can manage simultaneously.
     * @param num The max number of sets
     */
    VulkanDescriptorPoolBuilder& SetMaxSets(uint32_t num);
    
    /**
     * @brief Add the free descriptor set flag.
     * Then the descriptor pool allow to free descriptor sets individually.
     */
    VulkanDescriptorPoolBuilder& AllowFreeDescriptorSet();

private:
    bool allowFreeDescriptorSet{false};
    uint32_t maxSets{0};
    std::vector<VkDescriptorPoolSize> poolSizes;
};

#endif // VULKANDESCRIPTORPOOLBUILDER_H