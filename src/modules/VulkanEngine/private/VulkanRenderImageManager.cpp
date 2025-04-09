#include "VulkanRenderImageManager.h"

#include <stdexcept>

#include "VulkanRenderPassManager.h"

VulkanRenderImageManager* VulkanRenderImageManager::instance{nullptr};


VulkanRenderImageManager *VulkanRenderImageManager::GetInstance()
{
    if (!instance)
        instance = new VulkanRenderImageManager();

    return instance;
}

void VulkanRenderImageManager::Init(VkDevice device, VkPhysicalDevice physicalDevice)
{
    this->device = device;
    this->physicalDevice = physicalDevice;
}

void VulkanRenderImageManager::CreateImage(VkImage &image, VkDeviceMemory &imageMemory, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags flags) const
{
    if (device == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to create color image : invalid device");

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
    if (device == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to create color image view : invalid device");
    
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

void VulkanRenderImageManager::CreateColorImage(VkImage &colorImage, VkDeviceMemory &colorImageMemory, uint32_t width, uint32_t height) const
{

    if (device == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to create color image : invalid device");

    // IMAGE
    VkImageCreateInfo colorImageInfo = {};
    colorImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    colorImageInfo.imageType = VK_IMAGE_TYPE_2D;
    colorImageInfo.format = VulkanRenderPassManager::GetInstance()->GetColorFormat();  // Format de couleur
    colorImageInfo.extent.width = width;
    colorImageInfo.extent.height = height;
    colorImageInfo.extent.depth = 1;
    colorImageInfo.mipLevels = 1;
    colorImageInfo.arrayLayers = 1;
    colorImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    colorImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    colorImageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    colorImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    colorImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (vkCreateImage(device, &colorImageInfo, nullptr, &colorImage) != VK_SUCCESS)
        throw std::runtime_error("Failed to create color image!");

    // IMAGE MEMORY
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, colorImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
        memRequirements.memoryTypeBits, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    if (vkAllocateMemory(device, &allocInfo, nullptr, &colorImageMemory) != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate color image memory!");

    vkBindImageMemory(device, colorImage, colorImageMemory, 0);
}

void VulkanRenderImageManager::CreateColorImageView(VkImageView &colorImageView, VkImage &colorImage) const
{
    if (device == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to create color image view : invalid device");
    
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = colorImage;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = VulkanRenderPassManager::GetInstance()->GetColorFormat();
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &createInfo, nullptr, &colorImageView) != VK_SUCCESS)
        throw std::runtime_error("Failed to create color image views!");
}

void VulkanRenderImageManager::CreateDepthStencilImage(VkImage &depthImage, VkDeviceMemory &depthImageMemory, uint32_t width, uint32_t height) const
{
    if (device == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to create depth stencil image : invalid device");
    
    /* IMAGE */
    VkImageCreateInfo depthImageInfo = {};
    depthImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    depthImageInfo.imageType = VK_IMAGE_TYPE_2D;
    depthImageInfo.format = VulkanRenderPassManager::GetInstance()->GetDepthStencilFormat();  // Format de profondeur
    depthImageInfo.extent.width = width;
    depthImageInfo.extent.height = height;
    depthImageInfo.extent.depth = 1;
    depthImageInfo.mipLevels = 1;
    depthImageInfo.arrayLayers = 1;
    depthImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    depthImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    depthImageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    depthImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    depthImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (vkCreateImage(device, &depthImageInfo, nullptr, &depthImage) != VK_SUCCESS)
        throw std::runtime_error("Failed to create depth image!");

    /* IMAGE MEMORY */

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, depthImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
        memRequirements.memoryTypeBits, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    if (vkAllocateMemory(device, &allocInfo, nullptr, &depthImageMemory) != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate depth image memory!");

    vkBindImageMemory(device, depthImage, depthImageMemory, 0);
}

void VulkanRenderImageManager::CreateDepthStencilImageView(VkImageView &depthImageView, VkImage &depthImage) const
{
    if (device == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to create depth stencil image : invalid device");
    
    /* IMAGE VIEW */
    VkImageViewCreateInfo depthImageViewInfo = {};
    depthImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthImageViewInfo.image = depthImage;
    depthImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthImageViewInfo.format = VulkanRenderPassManager::GetInstance()->GetDepthStencilFormat();
    depthImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    depthImageViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    depthImageViewInfo.subresourceRange.baseMipLevel = 0;
    depthImageViewInfo.subresourceRange.levelCount = 1;
    depthImageViewInfo.subresourceRange.baseArrayLayer = 0;
    depthImageViewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &depthImageViewInfo, nullptr, &depthImageView) != VK_SUCCESS)
        throw std::runtime_error("Failed to create depth image view!");
}

uint32_t VulkanRenderImageManager::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
    if (physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to find memory : invalid physical device");


    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

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
