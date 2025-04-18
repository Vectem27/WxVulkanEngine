#ifndef VULKANSTORAGEBUFFER_H
#define VULKANSTORAGEBUFFER_H

#include "VulkanGpuOnlyBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanStorageBuffer 
 * @extends VulkanGpuOnlyBuffer
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
 */
class VulkanStorageBuffer : public VulkanGpuOnlyBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanStorageBuffer(bool isTransferDestination = TRANSFER_DEST_BUFFER)
        : VulkanGpuOnlyBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, isTransferDestination)
    {}

    VulkanStorageBuffer(VulkanStorageBuffer& other) = delete;
    VulkanStorageBuffer& operator=(VulkanStorageBuffer& other) = delete;

    VulkanStorageBuffer(VulkanStorageBuffer&& other) = default;
    VulkanStorageBuffer& operator=(VulkanStorageBuffer&& other) = default;

    virtual ~VulkanStorageBuffer() = default;
};

#endif // VULKANSTORAGEBUFFER_H