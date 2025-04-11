#ifndef VULKANINDEXBUFFER_H
#define VULKANINDEXBUFFER_H

#include "VulkanBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanIndexBuffer 
 * @extends VulkanBuffer
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
 * @date 11 apr 2025
 * @version 1.0
 */
class VulkanIndexBuffer : public VulkanBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanIndexBuffer() 
        : VulkanBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {}

    VulkanIndexBuffer(VulkanIndexBuffer& other) = delete;
    VulkanIndexBuffer& operator=(VulkanIndexBuffer& other) = delete;

    VulkanIndexBuffer(VulkanIndexBuffer&& other) = default;
    VulkanIndexBuffer& operator=(VulkanIndexBuffer&& other) = default;

    virtual ~VulkanIndexBuffer() = default;
};

#endif // VULKANINDEXBUFFER_H