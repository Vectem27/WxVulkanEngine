#include "VulkanRenderTarget.h"

#include <vector>
#include <stdexcept>

#include "Logger.h"
#include "VulkanDeviceManager.h"
#include "VulkanRenderPassManager.h"

void VulkanRenderTarget::Init(uint32_t width, uint32_t height)
{
    this->width = width;
    this->height = height;

    Cleanup();

    baseColorTexture.InitTexture(
        GetWidth(), GetHeight(), 
        GetVulkanRenderPassManager().GetColorFormat(),
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    depthStencilTexture.InitTexture(
        GetWidth(), GetHeight(), 
        GetVulkanRenderPassManager().GetDepthStencilFormat(),
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
    );

    normalTexture.InitTexture(
        GetWidth(), GetHeight(), 
        GetVulkanRenderPassManager().GetColorFormat(),
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    positionTexture.InitTexture(
        GetWidth(), GetHeight(), 
        GetVulkanRenderPassManager().GetHDRFormat(),
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    lightingTexture.InitTexture(
        GetWidth(), GetHeight(), 
        GetVulkanRenderPassManager().GetHDRFormat(),
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    postprocessTexture.InitTexture(
        GetWidth(), GetHeight(), 
        GetVulkanRenderPassManager().GetColorFormat(),
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    CreateFrameBuffer();
}

void VulkanRenderTarget::Cleanup() noexcept
{
    baseColorTexture.Cleanup();
    depthStencilTexture.Cleanup();
    normalTexture.Cleanup();
    positionTexture.Cleanup();
    lightingTexture.Cleanup();
    postprocessTexture.Cleanup();

    CleanupFrameBuffer();
}

void VulkanRenderTarget::CleanupFrameBuffer() noexcept
{
    try
    {
        if (geometryFrameBuffer != VK_NULL_HANDLE) 
            vkDestroyFramebuffer(GetVulkanDeviceManager().GetDeviceChecked(), geometryFrameBuffer, nullptr);

        if (lightingFrameBuffer != VK_NULL_HANDLE)
            vkDestroyFramebuffer(GetVulkanDeviceManager().GetDeviceChecked(), lightingFrameBuffer, nullptr);

        if (postprocessFrameBuffer != VK_NULL_HANDLE)
            vkDestroyFramebuffer(GetVulkanDeviceManager().GetDeviceChecked(), postprocessFrameBuffer, nullptr);
    }
    catch(...)
    {
        Log(Warning, "Vulkan", "Failed to cleanup render target");
    }

    geometryFrameBuffer = VK_NULL_HANDLE;
    lightingFrameBuffer = VK_NULL_HANDLE;
    postprocessFrameBuffer = VK_NULL_HANDLE;
}

void VulkanRenderTarget::CreateFrameBuffer()
{
    CleanupFrameBuffer();

    std::vector<VkImageView> attachments = 
    {
        baseColorTexture.GetImageView(), 
        depthStencilTexture.GetImageView(),
        normalTexture.GetImageView(),
        positionTexture.GetImageView(),
    };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = GetVulkanRenderPassManager().GetGeometryPass();
    framebufferInfo.attachmentCount = attachments.size();
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = GetWidth();
    framebufferInfo.height = GetHeight();
    framebufferInfo.layers = 1;

    VkResult result;
    result = vkCreateFramebuffer(
        GetVulkanDeviceManager().GetDeviceChecked(), 
        &framebufferInfo, nullptr, &GetGeometryFrameBuffer()
    );
    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to create geometry framebuffer, result code : %d", result);
        throw std::runtime_error("Failed to create geometry framebuffer!");
    }


    std::vector<VkImageView> lightingAttachments = 
    {
        lightingTexture.GetImageView(),
    };

    framebufferInfo={};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = VulkanRenderPassManager::GetInstance()->GetLightingPass();
    framebufferInfo.attachmentCount = static_cast<uint32_t>(lightingAttachments.size());
    framebufferInfo.pAttachments = lightingAttachments.data();
    framebufferInfo.width = GetWidth();
    framebufferInfo.height = GetHeight();
    framebufferInfo.layers = 1;

    result = vkCreateFramebuffer(
        GetVulkanDeviceManager().GetDeviceChecked(), 
        &framebufferInfo, nullptr, &GetLightingFrameBuffer()
    );
    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to create lighting framebuffer, result code : %d", result);
        throw std::runtime_error("Failed to create lighting framebuffer!");
    }

    /* POST PROCESS*/
    std::vector<VkImageView> postprocessAttachments = 
    {
        postprocessTexture.GetImageView(),
    };

    framebufferInfo={};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = VulkanRenderPassManager::GetInstance()->GetPostprocessPass();
    framebufferInfo.attachmentCount = static_cast<uint32_t>(postprocessAttachments.size());
    framebufferInfo.pAttachments = postprocessAttachments.data();
    framebufferInfo.width = GetWidth();
    framebufferInfo.height = GetHeight();
    framebufferInfo.layers = 1;

    vkCreateFramebuffer(
        GetVulkanDeviceManager().GetDeviceChecked(), 
        &framebufferInfo, nullptr, &GetPostprocessFrameBuffer()
    );

    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to create post-process framebuffer, result code : %d", result);
        throw std::runtime_error("Failed to create post-process framebuffer!");
    }
}
