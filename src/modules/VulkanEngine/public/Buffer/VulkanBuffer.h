#ifndef VULKANBUFFER_H
#define VULKANBUFFER_H

#include <vulkan/vulkan.h>


/**
 * @class VulkanBuffer
 * @ingroup Buffer
 * 
 * @brief Generic class for the vulkan buffer managment
 * 
 * @author Vectem
 * @date 11 apr 2025
 * @version 1.0
 */
class VulkanBuffer
{
public:
    /**
     * @brief The VulkanBuffer constructor
     * 
     * @param usage Sepecify the buffer usage flags
     * @param properties Specify the memory properties flags
     */
    VulkanBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

    /**
     * @brief Semantic move constructor of VulkanBuffer
     */
    VulkanBuffer(VulkanBuffer&& other) noexcept;

    /**
     * @brief Semantic move operator of VulkanBuffer
    */
    VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    virtual ~VulkanBuffer() { Cleanup(); }

public:
    VulkanBuffer() = delete;
    VulkanBuffer(VulkanBuffer& other) = delete;
    VulkanBuffer& operator=(VulkanBuffer& other) = delete;
    
public:
    /**
     * @brief Create the buffer and allocate the memory
     * @param size Number of bytes to allocate
     */
    void Create(VkDeviceSize size);

    

    /**
     * @brief Cleanup the buffer memory
     */
    void Cleanup() noexcept;

    /**
     * @brief Getter for the vulkan buffer
     * @return Return the VkBuffer
     */
    VkBuffer& GetBuffer() { return buffer; }
    VkBuffer GetBuffer() const { return buffer; }

    /**
     * @brief Getter for the buffer size
     * @return Return the buffer size
     */
    VkDeviceSize GetBufferSize() const { return bufferSize; }

protected:
    VkBuffer buffer{VK_NULL_HANDLE};
    VkDeviceMemory bufferMemory{VK_NULL_HANDLE};
    VkDeviceSize bufferSize{0};
    
private:
    VkBufferUsageFlags bufferUsage;
    VkMemoryPropertyFlags memoryProperties;
};


#endif // VULKANBUFFER_H