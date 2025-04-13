#ifndef VULKANCOMMANDPOOLBUILDER_H
#define VULKANCOMMANDPOOLBUILDER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <inttypes.h>

/**
 * @brief The command pool creation helper class
 * @class VulkanCommandPoolBuilder
 * 
 * Command pool creation example :
 * @code
 * commandPool = VulkanCommandPoolBuilder().AllowResetCommandBuffer().SetQueueFamilyIndex(index).Build();
 * @endcode
 * 
 * @author Vectem
 */
class VulkanCommandPoolBuilder
{
public:
    VulkanCommandPoolBuilder() = default;
    VulkanCommandPoolBuilder(const VulkanCommandPoolBuilder&) = delete;
    VulkanCommandPoolBuilder& operator=(const VulkanCommandPoolBuilder&) = delete;
    VulkanCommandPoolBuilder(VulkanCommandPoolBuilder&&) noexcept = delete;
    VulkanCommandPoolBuilder& operator=(VulkanCommandPoolBuilder&&) noexcept = delete;
    ~VulkanCommandPoolBuilder() = default;
public:
    /**
     * @brief Reset the command pool builder for another use
     */
    VulkanCommandPoolBuilder& Reset();

    /**
     * @brief Build function
     * @return The created command pool
     */
    VkCommandPool Build();

    /**
     * @brief Set the used queue family index
     */
    VulkanCommandPoolBuilder& SetQueueFamilyIndex(uint32_t queueFamilyIndex);

    /**
     * @brief Add the command buffer reset flag
     */
    VulkanCommandPoolBuilder& AllowResetCommandBuffer();

    /**
     * @brief Add the transient flag
     */
    VulkanCommandPoolBuilder& MarkTransient();

    /**
     * @brief Add the protected flag
     */
    VulkanCommandPoolBuilder& MarkProtected();
    
private:
    bool allowResetCommand{false};
    bool transient{false};
    bool protec{false};
    uint32_t queueFamilyIndex{VK_QUEUE_FAMILY_IGNORED};
};

#endif // VULKANCOMMANDPOOLBUILDER_H