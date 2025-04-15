#ifndef VULKANCPUONLYBUFFER_H
#define VULKANCPUONLYBUFFER_H

#include "VulkanBuffer.h"

class VulkanCpuOnlyBuffer : public VulkanBuffer
{
public:
    VulkanCpuOnlyBuffer(VkBufferUsageFlags usage) 
        : VulkanBuffer(usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    {}

    VulkanCpuOnlyBuffer(VulkanCpuOnlyBuffer& other) = delete;
    VulkanCpuOnlyBuffer& operator=(VulkanCpuOnlyBuffer& other) = delete;

    VulkanCpuOnlyBuffer(VulkanCpuOnlyBuffer&& other) = default;
    VulkanCpuOnlyBuffer& operator=(VulkanCpuOnlyBuffer&& other) = default;

    virtual ~VulkanCpuOnlyBuffer() = default;

public:

    /**
     * @brief Update the buffer memory
     * @param data New buffer memory data
     */
    void Update(const void* data) { Update(data, 0, static_cast<size_t>(bufferSize)); }

    /**
     * @brief Update the buffer memory
     * @param data New data
     * @param offset Starting offset (in bytes)
     * @param size Number of bytes to update
     */
    void Update(const void* data, VkDeviceSize offset, size_t size);
};

#endif // VULKANCPUONLYBUFFER_H