#ifndef VULKANGPUONLYBUFFER_H
#define VULKANGPUONLYBUFFER_H

#include "VulkanBuffer.h"

/**
 * @brief Set the buffer as transfer destination usage
 */
constexpr bool TRANSFER_DEST_BUFFER{true};

/**
 * @brief Set the buffer as not transfer destination usage
 */
constexpr bool NOT_TRANSFER_DEST_BUFFER{true};


class VulkanGpuOnlyBuffer : public VulkanBuffer
{
public:
    VulkanGpuOnlyBuffer(VkBufferUsageFlags usage, bool isTransferDestination) 
        : VulkanBuffer(isTransferDestination ? usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT : usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {}

    VulkanGpuOnlyBuffer(VulkanGpuOnlyBuffer& other) = delete;
    VulkanGpuOnlyBuffer& operator=(VulkanGpuOnlyBuffer& other) = delete;

    VulkanGpuOnlyBuffer(VulkanGpuOnlyBuffer&& other) = default;
    VulkanGpuOnlyBuffer& operator=(VulkanGpuOnlyBuffer&& other) = default;

    virtual ~VulkanGpuOnlyBuffer() = default;
};


#endif // VULKANGPUONLYBUFFER_H