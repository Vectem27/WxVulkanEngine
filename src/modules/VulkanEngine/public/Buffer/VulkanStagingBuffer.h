#ifndef VULKANSTAGINGBUFFER_H
#define VULKANSTAGINGBUFFER_H

#include "VulkanBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanStagingBuffer 
 * @extends VulkanBuffer
 * @ingroup Buffer
 * 
 * @brief The vulkan index buffer implementation
 * 
 * Exemple :
 * @code
 * std::vector<int> content;
 * VulkanStagingBuffer buffer;
 * buffer.Create(content.size() * sizeof(int));
 * buffer.Update(content.data());
 * @endcode
 * 
 * @author Vectem
 * @date 11 apr 2025
 * @version 1.0
 */
class VulkanStagingBuffer : public VulkanBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanStagingBuffer() 
        : VulkanBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    {}

    VulkanStagingBuffer(VulkanStagingBuffer& other) = delete;
    VulkanStagingBuffer& operator=(VulkanStagingBuffer& other) = delete;

    VulkanStagingBuffer(VulkanStagingBuffer&& other) = default;
    VulkanStagingBuffer& operator=(VulkanStagingBuffer&& other) = default;

    virtual ~VulkanStagingBuffer() = default;
};

#endif // VULKANSTAGINGBUFFER_H