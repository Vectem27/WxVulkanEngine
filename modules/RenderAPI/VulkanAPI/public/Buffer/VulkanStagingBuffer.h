#ifndef VULKANSTAGINGBUFFER_H
#define VULKANSTAGINGBUFFER_H

#include "VulkanCpuOnlyBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanStagingBuffer 
 * @extends VulkanCpuOnlyBuffer
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
 */
class VulkanStagingBuffer : public VulkanCpuOnlyBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanStagingBuffer() 
        : VulkanCpuOnlyBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
    {}

    VulkanStagingBuffer(VulkanStagingBuffer& other) = delete;
    VulkanStagingBuffer& operator=(VulkanStagingBuffer& other) = delete;

    VulkanStagingBuffer(VulkanStagingBuffer&& other) = default;
    VulkanStagingBuffer& operator=(VulkanStagingBuffer&& other) = default;

    virtual ~VulkanStagingBuffer() = default;
};

#endif // VULKANSTAGINGBUFFER_H