#ifndef VULKANSTORAGEBUFFER_H
#define VULKANSTORAGEBUFFER_H

#include "VulkanBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanStorageBuffer 
 * @extends VulkanBuffer
 * @ingroup Buffer
 * 
 * @brief The vulkan storage buffer implementation
 * 
 * Exemple :
 * @code
 * std::vector<int> content;
 * VulkanStorageBuffer buffer;
 * buffer.Create(content.size() * sizeof(int));
 * buffer.Update(content.data());
 * @endcode
 * 
 * @author Vectem
 * @date 11 apr 2025
 * @version 1.0
 */
class VulkanStorageBuffer : public VulkanBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanStorageBuffer(BufferType bufferType = BufferType::DEVICE_LOCAL)
        : VulkanBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            HOST_VISIBLE_BIT | HOST_COHERENT_BIT)
    {}

    VulkanStorageBuffer(VulkanStorageBuffer& other) = delete;
    VulkanStorageBuffer& operator=(VulkanStorageBuffer& other) = delete;

    VulkanStorageBuffer(VulkanStorageBuffer&& other) = default;
    VulkanStorageBuffer& operator=(VulkanStorageBuffer&& other) = default;

    virtual ~VulkanStorageBuffer() = default;
};

#endif // VULKANSTORAGEBUFFER_H