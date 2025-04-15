#ifndef VULKANINDEXBUFFER_H
#define VULKANINDEXBUFFER_H

#include "VulkanGpuOnlyBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanIndexBuffer 
 * @extends VulkanGpuOnlyBuffer
 * @ingroup Buffer
 * 
 * @brief The vulkan index buffer implementation
 * 
 * Exemple :
 * @code
 * std::vector<int> content;
 * VulkanIndexBuffer buffer;
 * buffer.Create(content.size() * sizeof(int));
 * buffer.Update(content.data());
 * @endcode
 * 
 * @author Vectem
 */
class VulkanIndexBuffer : public VulkanGpuOnlyBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanIndexBuffer() 
        : VulkanGpuOnlyBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, TRANSFER_DEST_BUFFER)
    {}

    VulkanIndexBuffer(VulkanIndexBuffer& other) = delete;
    VulkanIndexBuffer& operator=(VulkanIndexBuffer& other) = delete;

    VulkanIndexBuffer(VulkanIndexBuffer&& other) = default;
    VulkanIndexBuffer& operator=(VulkanIndexBuffer&& other) = default;

    virtual ~VulkanIndexBuffer() = default;
};

#endif // VULKANINDEXBUFFER_H