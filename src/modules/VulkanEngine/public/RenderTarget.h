#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

class RenderTarget {
public:
    RenderTarget(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, VkFormat format, VkRenderPass renderPass)
        : device(device), extent(extent) {
        createImage(physicalDevice, format);
        createImageView(format);
        createFramebuffer(renderPass);
    }

    ~RenderTarget() 
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
        vkDestroyImageView(device, imageView, nullptr);
        vkDestroyImage(device, image, nullptr);
        vkFreeMemory(device, imageMemory, nullptr);
    }

    VkFramebuffer getFramebuffer() const { return framebuffer; }
    VkImageView getImageView() const { return imageView; }
    VkImage getImage() const { return image; }

private:
    VkDevice device;
    VkExtent2D extent;
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkFramebuffer framebuffer;

    void createImage(VkPhysicalDevice physicalDevice, VkFormat format) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent = {extent.width, extent.height, 1};
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        vkCreateImage(device, &imageInfo, nullptr, &image);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory);
        vkBindImageMemory(device, image, imageMemory, 0);
    }

    void createImageView(VkFormat format) {
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

        vkCreateImageView(device, &viewInfo, nullptr, &imageView);
    }

    void createFramebuffer(VkRenderPass renderPass) {
        VkImageView attachments[] = { imageView };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer);
    }

    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type!");
    }
};


#endif // RENDERTARGET_H