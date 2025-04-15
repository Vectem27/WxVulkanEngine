#include "VulkanRenderImageManager.h"

#include <stdexcept>

#include "VulkanRenderPassManager.h"
#include "VulkanDeviceManager.h"


void VulkanRenderImageManager::CreateImage(VkImage &image, VkDeviceMemory &imageMemory, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags flags) const
{
    auto device = GetVulkanDeviceManager().GetDeviceChecked();
    // IMAGE
    VkImageCreateInfo colorImageInfo = {};
    colorImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    colorImageInfo.imageType = VK_IMAGE_TYPE_2D;
    colorImageInfo.format = format;  // Format de couleur
    colorImageInfo.extent.width = width;
    colorImageInfo.extent.height = height;
    colorImageInfo.extent.depth = 1;
    colorImageInfo.mipLevels = 1;
    colorImageInfo.arrayLayers = 1;
    colorImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    colorImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    colorImageInfo.usage = flags;
    colorImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    colorImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (vkCreateImage(device, &colorImageInfo, nullptr, &image) != VK_SUCCESS)
        throw std::runtime_error("Failed to create color image!");

    // IMAGE MEMORY
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
        memRequirements.memoryTypeBits, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate color image memory!");

    vkBindImageMemory(device, image, imageMemory, 0);
}

void VulkanRenderImageManager::CreateImageView(VkImageView &imageView, VkImage &image, VkFormat format, VkImageAspectFlags flags) const
{
    auto device = GetVulkanDeviceManager().GetDeviceChecked();

    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = flags;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &createInfo, nullptr, &imageView) != VK_SUCCESS)
        throw std::runtime_error("Failed to create color image views!");
}

uint32_t VulkanRenderImageManager::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(GetVulkanDeviceManager().GetPhysicalDeviceChecked(), &memProperties);

    // Parcourir tous les types de mémoire pour trouver celui qui correspond aux critères
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}