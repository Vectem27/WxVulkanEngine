#ifndef VULKANCOMMANDUTILS_H
#define VULKANCOMMANDUTILS_H

#include <vulkan/vulkan.h>

/**
 * @namespace VulkanCommandUtils
 * @brief Contain vulkan command buffer/pool utility functions
 */
namespace VulkanCommandUtils
{
    /**
     * @brief Create and allocate command buffer
     * @param commandPool The command pool
     * @param level Primary or secondary command buffer
     * 
     * @return The created command buffer
     * @note Parameters are checked and can throw a runtime error
     */
    VkCommandBuffer AllocateCommandBuffer(
        VkCommandPool commandPool, 
        VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
    );

    /**
     * @brief Reset a command buffer 
     * @param commandBuffer The command buffer to reset
     * @note Parameters are checked and can throw a runtime error
     * @warning This function should be called on a command buffer created by a pool with the reset command flag
     */
    void ResetCommandBuffer(VkCommandBuffer commandBuffer);

    /**
     * @brief Begin a command buffer 
     * @param commandBuffer The command buffer to begin
     * 
     * @note Parameters are checked and can throw a runtime error
     * @note The buffer begin with de simultaneous use flag
     */
    void BeginCommandBuffer(VkCommandBuffer commandBuffer);

    /**
     * @brief End a command buffer
     * @param commandBuffer The command buffer to end
     * 
     * @note Parameters are checked and can throw a runtime error
     */
    void EndCommandBuffer(VkCommandBuffer commandBuffer);

    /**
     * @brief Submit a command buffer with default options
     * @param queue The queue to submit commands
     * @param commandBuffer The command buffer to submit
     * 
     * @note Parameters are checked and can throw a runtime error
     * @warning If you need semaphores syncronisation, don't use this function
     */
    void SubmitCommandBuffer(VkQueue queue, const VkCommandBuffer commandBuffer);

    /**
     * @brief Submit a command buffer with semaphore synchronization
     * @param queue The queue to submit commands
     * @param commandBuffer The command buffer to submit
     * @param waitSemaphore The semaphore to wait on before starting the execution of the command buffer
     * @param signalSemaphore The semaphore to signal after the execution of the command buffer
     * 
     * @note Parameters are checked and can throw a runtime error
     * @note Use this function when you need synchronization between commands via semaphores.
     */
    void SubmitCommandBufferWithSync(
        VkQueue queue, 
        const VkCommandBuffer commandBuffer, 
        VkSemaphore waitSemaphore,
        VkSemaphore signalSemaphore,
        VkFence fence
    );


    /**
     * @brief Free a command buffer
     * @param commandPool The command pool which created the command buffer
     * @param commandBuffer The command buffer to submit
     * 
     * @note Parameters are checked but don't throw errors
     */
    void FreeCommandBuffer(VkCommandPool commandPool, VkCommandBuffer& commandBuffer) noexcept;

    /**
     * @brief Destroy a command pool
     * @param commandPool The command pool to destroy
     * 
     * @note Parameters are checked but don't throw errors
     */
    void DestroyCommandPool(VkCommandPool& commandPool) noexcept;
};

#endif // VULKANCOMMANDUTILS_H