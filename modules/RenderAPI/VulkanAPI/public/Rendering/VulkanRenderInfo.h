#ifndef VULKANRENDERINFO_H
#define VULKANRENDERINFO_H

#include <vulkan/vulkan.h>

class VulkanRenderInfo 
{
public:
    VulkanRenderInfo() = default;
    ~VulkanRenderInfo() = default;

    void SetRenderPass(VkRenderPass renderPass) { this->renderPass = renderPass; }
    void SetFramebuffer(VkFramebuffer framebuffer) { this->framebuffer = framebuffer; }
    void SetCommandBuffer(VkCommandBuffer commandBuffer) { this->commandBuffer = commandBuffer; }
    void SetClearValues(VkClearValue* clearValues, uint32_t clearValuesCount) { this->clearValues = clearValues; this->clearValuesCount = clearValuesCount;}

    VkRenderPass GetRenderPass() const { return renderPass; }
    VkFramebuffer GetFramebuffer() const { return framebuffer; }
    VkCommandBuffer GetCommandBuffer() const { return commandBuffer; }
    VkClearValue* GetClearValues() const { return clearValues; }
    uint32_t GetClearValuesCount() const { return clearValuesCount; }
    
private:
    VkRenderPass renderPass{ VK_NULL_HANDLE };
    VkFramebuffer framebuffer{ VK_NULL_HANDLE };
    VkCommandBuffer commandBuffer{ VK_NULL_HANDLE };
    VkClearValue* clearValues{nullptr};
    uint32_t clearValuesCount{0};
    
};

#endif // VULKANRENDERINFO_H