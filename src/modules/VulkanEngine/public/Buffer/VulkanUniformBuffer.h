#ifndef VULKANUNIFORMBUFFER_H
#define VULKANUNIFORMBUFFER_H

#include "VulkanBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanUniformBuffer 
 * @extends VulkanBuffer
 * @ingroup Buffer
 * 
 * @brief The vulkan uniform buffer implementation
 * 
 * Exemple :
 * @code
 * std::vector<int> content;
 * VulkanUniformBuffer buffer;
 * buffer.Create(content.size() * sizeof(int));
 * buffer.Update(content.data());
 * @endcode
 * 
 * @author Vectem
 * @date 11 apr 2025
 * @version 1.0
 */
class VulkanUniformBuffer : public VulkanBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanUniformBuffer() 
        : VulkanBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    {}

    VulkanUniformBuffer(VulkanUniformBuffer& other) = delete;
    VulkanUniformBuffer& operator=(VulkanUniformBuffer& other) = delete;

    VulkanUniformBuffer(VulkanUniformBuffer&&) noexcept = default;
    VulkanUniformBuffer& operator=(VulkanUniformBuffer&&) noexcept = default;

    virtual ~VulkanUniformBuffer() = default;
};

#endif // VULKANUNIFORMBUFFER_H