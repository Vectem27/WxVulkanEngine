#ifndef VULKANRENDERINFO_H
#define VULKANRENDERINFO_H

// Libraries
#include <vulkan/vulkan.h>
// TODO: Set this file to private

namespace Vulkan
{
    class RenderCamera;
    class SceneRenderer;

    struct RenderInfo
    {
        VkCommandBuffer commandBuffer{VK_NULL_HANDLE};

        RenderCamera* camera{nullptr};
    };
}

#endif // VULKANRENDERINFO_H