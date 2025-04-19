#include "VulkanRenderMaterial.h"

#include "Logger.h"

void Vulkan::VulkanRenderMaterial::BindRenderMaterial(VkCommandBuffer commandBuffer) const
{
    if (commandBuffer == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to bind render material : command buffer is null");
        throw std::runtime_error("Failed to bind render material : command buffer is null");
        return;
    }

    if (renderPipeline == VK_NULL_HANDLE)
    {
        Log(Error, "Vulkan", "Failed to bind render material : render pipeline is null");
        throw std::runtime_error("Failed to bind render material : render pipeline is null");
        return;
    }

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderPipeline);
}