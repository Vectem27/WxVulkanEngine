#include "VulkanShadowMap.h"

#include <stdexcept>
#include "Logger.h"
#include "VulkanCommandUtils.h"
#include "VulkanDeviceManager.h"
#include "VulkanRenderPassManager.h"
#include "VulkanTexture.h"

//TODO: Change shadow maps format with only depth format

VulkanShadowMap::VulkanShadowMap(uint32_t resolution)
    : resolution(resolution), format(GetVulkanRenderPassManager().GetDepthStencilFormat())
{
    depthTexture = new VulkanTexture();
    depthTexture->InitTexture(
        GetResolution(), GetResolution(), format, 
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT
    );

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = GetVulkanRenderPassManager().GetShadowPass();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &GetImageView();
    framebufferInfo.width = GetResolution();
    framebufferInfo.height = GetResolution();
    framebufferInfo.layers = 1;

    if (auto result = vkCreateFramebuffer(GetVulkanDeviceManager().GetDeviceChecked(), &framebufferInfo, nullptr, &frameBuffer); result != VK_SUCCESS) 
    {
        Log(Error, "Vulkan", "Failed to create shadow map framebuffer. Result code : %d", result);
        throw std::runtime_error("Failed to create shadow map framebuffer");
    }
}

VulkanShadowMap::~VulkanShadowMap()
{
    vkDestroyFramebuffer(GetVulkanDeviceManager().GetDeviceChecked(), frameBuffer, nullptr);
    delete depthTexture;
}

void VulkanShadowMap::BeginRendering(VkCommandBuffer commandBuffer)
{
    VulkanCommandUtils::ResetCommandBuffer(commandBuffer);
    VulkanCommandUtils::BeginCommandBuffer(commandBuffer);
}

void VulkanShadowMap::EndRendering(VkQueue queue, VkCommandBuffer commandBuffer)
{
    VulkanCommandUtils::EndCommandBuffer(commandBuffer);
    VulkanCommandUtils::SubmitCommandBuffer(GetVulkanDeviceManager().GetGraphicsQueue(), commandBuffer);
}

VkImageView VulkanShadowMap::GetImageView() const
{
    return depthTexture->GetImageView();
}

VkImageView &VulkanShadowMap::GetImageView()
{
    return depthTexture->GetImageView();
}
