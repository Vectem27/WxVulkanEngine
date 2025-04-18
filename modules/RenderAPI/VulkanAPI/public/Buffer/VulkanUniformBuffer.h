#ifndef VULKANUNIFORMBUFFER_H
#define VULKANUNIFORMBUFFER_H

#include "VulkanCpuOnlyBuffer.h"
#include <utility> // std::move

/**
 * @class VulkanUniformBuffer 
 * @extends VulkanCpuOnlyBuffer
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
 */
class VulkanUniformBuffer : public VulkanCpuOnlyBuffer
{
public:
    /**
     * @brief The default constructor
     */
    VulkanUniformBuffer() 
        : VulkanCpuOnlyBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
    {}

    VulkanUniformBuffer(VulkanUniformBuffer& other) = delete;
    VulkanUniformBuffer& operator=(VulkanUniformBuffer& other) = delete;

    VulkanUniformBuffer(VulkanUniformBuffer&&) noexcept = default;
    VulkanUniformBuffer& operator=(VulkanUniformBuffer&&) noexcept = default;

    virtual ~VulkanUniformBuffer() = default;
};

#endif // VULKANUNIFORMBUFFER_H