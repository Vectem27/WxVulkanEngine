#ifndef VULKANINDIRECTBUFFER_H
#define VULKANINDIRECTBUFFER_H

#include "VulkanBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanIndirectBuffer 
 * @extends VulkanBuffer
 * @ingroup Buffer
 * 
 * @brief The vulkan indirect buffer implementation
 * 
 * Exemple :
 * @code
 * std::vector<int> content;
 * VulkanIndirectBuffer buffer;
 * buffer.Create(content.size() * sizeof(int));
 * buffer.Update(content.data());
 * @endcode
 * 
 * @author Vectem
 * @date 11 apr 2025
 * @version 1.0
 */
class VulkanIndirectBuffer : public VulkanBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanIndirectBuffer() 
        : VulkanBuffer(VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {}

    VulkanIndirectBuffer(VulkanIndirectBuffer& other) = delete;
    VulkanIndirectBuffer& operator=(VulkanIndirectBuffer& other) = delete; 
    
    VulkanIndirectBuffer(VulkanIndirectBuffer&& other) = default;
    VulkanIndirectBuffer& operator=(VulkanIndirectBuffer&& other) = default; 
    

    virtual ~VulkanIndirectBuffer() = default;
};

#endif // VULKANINDIRECTBUFFER_H