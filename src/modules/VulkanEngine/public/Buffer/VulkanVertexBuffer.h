#ifndef VULKANVERTEXBUFFER_H
#define VULKANVERTEXBUFFER_H

#include "VulkanBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanVertexBuffer 
 * @extends VulkanBuffer
 * @ingroup Buffer
 * 
 * @brief The vulkan vertex buffer implementation
 * 
 * Exemple :
 * @code
 * std::vector<int> content;
 * VulkanVertexBuffer buffer;
 * buffer.Create(content.size() * sizeof(int));
 * buffer.Update(content.data());
 * @endcode
 * 
 * @author Vectem
 * @date 11 apr 2025
 * @version 1.0
 */
class VulkanVertexBuffer : public VulkanBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanVertexBuffer() 
        : VulkanBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {}

    VulkanVertexBuffer(VulkanVertexBuffer& other) = delete;
    VulkanVertexBuffer& operator=(VulkanVertexBuffer& other) = delete;

    VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept = default;
    VulkanVertexBuffer& operator=(VulkanVertexBuffer&& other) noexcept = default;

    virtual ~VulkanVertexBuffer() = default;
};

#endif // VULKANVERTEXBUFFER_H