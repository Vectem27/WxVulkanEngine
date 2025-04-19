#ifndef VULKANRENDERINFO_H
#define VULKANRENDERINFO_H

// Libraries
#include <vulkan/vulkan.h>

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