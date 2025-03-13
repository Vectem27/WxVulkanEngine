#include "VulkanRenderTarget.h"
#include "VulkanRenderEngine.h"

VulkanRenderTarget::VulkanRenderTarget(VulkanRenderEngine *renderEngine, uint32_t width, uint32_t height, VkFormat format)
    : renderEngine(renderEngine), width(width), height(height), format(format) 
{
    CreateImage();
    CreateImageView();
}

VulkanRenderTarget::~VulkanRenderTarget()
{
    vkDestroyFramebuffer(renderEngine->GetDevice(), framebuffer, nullptr);
    vkDestroyImageView(renderEngine->GetDevice(), imageView, nullptr);
    vkDestroyImage(renderEngine->GetDevice(), image, nullptr);
    vkFreeMemory(renderEngine->GetDevice(), imageMemory, nullptr);
}

void VulkanRenderTarget::CreateFramebuffer(VkRenderPass renderPass)
{
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &imageView;
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(renderEngine->GetDevice(), &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create render target framebuffer.");
    }
}

void VulkanRenderTarget::CreateImage()
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(renderEngine->GetDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create render target image.");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(renderEngine->GetDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = renderEngine->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(renderEngine->GetDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to allocate render target image memory.");
    }

    vkBindImageMemory(renderEngine->GetDevice(), image, imageMemory, 0);
}

void VulkanRenderTarget::CreateImageView()
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(renderEngine->GetDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create render target image view.");
    }
}
