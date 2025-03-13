#include "VulkanSwapchain.h"
#include "VulkanRenderEngine.h"
#include <algorithm>
#include <array>

VulkanSwapchain::VulkanSwapchain(VulkanRenderEngine *renderEngine, VkSurfaceKHR surface, uint32_t graphicsQueueFamilyIndex)
    : renderEngine(renderEngine), surface(surface)
{ 
    CreateCommandPool(graphicsQueueFamilyIndex);
    CreateSync();

    CreateSwapchain();
    CreateImageView();
}

VulkanSwapchain::~VulkanSwapchain()
{
    Cleanup();
    
    // Détruire les sémaphores et la fence
    if (imageAvailableSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(renderEngine->GetDevice(), imageAvailableSemaphore, nullptr);
        imageAvailableSemaphore = VK_NULL_HANDLE;
    }

        if (inFlightFence != VK_NULL_HANDLE) 
        {
            vkDestroyFence(renderEngine->GetDevice(), inFlightFence, nullptr);
            inFlightFence = VK_NULL_HANDLE;
        }
        if (renderFinishedSemaphore != VK_NULL_HANDLE) 
        {
            vkDestroySemaphore(renderEngine->GetDevice(), renderFinishedSemaphore, nullptr);
            renderFinishedSemaphore = VK_NULL_HANDLE;
        }

    // Détruire le command pool
    if (commandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(renderEngine->GetDevice(), commandPool, nullptr);
        commandPool = VK_NULL_HANDLE;
    }
}

void VulkanSwapchain::Create(VkRenderPass renderPass)
{
    this->renderPass = renderPass;
    CreateSwapchain();
    CreateDepthResources();
    CreateImageView();
    CreateFramebuffer();
    CreateCommandBuffers();
}

void VulkanSwapchain::SetRenderPass(VkRenderPass renderPass)
{
    static bool isFirst = true;
    if (isFirst)
    {
        Create(renderPass);
        isFirst = false;
    }
    else
    {
        this->renderPass = renderPass;
        Recreate();
    }
    
}

void VulkanSwapchain::Recreate()
{
    vkDeviceWaitIdle(renderEngine->GetDevice());

    Cleanup();

    CreateSwapchain();
    CreateDepthResources();
    CreateImageView();
    CreateFramebuffer();
    CreateCommandBuffers();
}

void VulkanSwapchain::CreateCommandPool(uint32_t graphicsQueueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    if (vkCreateCommandPool(renderEngine->GetDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

void VulkanSwapchain::CreateCommandBuffers()
{
    commandBuffers.resize(GetImageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(renderEngine->GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void VulkanSwapchain::CreateSync()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(renderEngine->GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(renderEngine->GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // La fence est initialement signalée
    if (vkCreateFence(renderEngine->GetDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create fence!");
    }
}

void VulkanSwapchain::CreateSwapchain()
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderEngine->GetPhysicalDevice(), surface, &capabilities);

    VkExtent2D extent = capabilities.currentExtent;
    if (extent.width == UINT32_MAX)
    {
        extent.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    }
    width = extent.width;
    height = extent.height;

    swapchainExtent = extent;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && createInfo.minImageCount > capabilities.maxImageCount)
    {
        createInfo.minImageCount = capabilities.maxImageCount;
    }
    createInfo.imageFormat = renderEngine->GetSwapChainImageFormat();
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = swapchainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(renderEngine->GetDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    imageCount;
    vkGetSwapchainImagesKHR(renderEngine->GetDevice(), swapchain, &imageCount, nullptr);
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(renderEngine->GetDevice(), swapchain, &imageCount, images.data());

}

void VulkanSwapchain::CreateImageView()
{
    imageViews.resize(images.size());
    for (size_t i = 0; i < images.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = renderEngine->GetSwapChainImageFormat();
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(renderEngine->GetDevice(), &createInfo, nullptr, &imageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views!");
        }
    }

    depthImageViews.resize(depthImages.size());
    for (size_t i = 0; i < depthImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = depthImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = renderEngine->GetDepthStencilImageFormat();
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        // Correction du masque d'aspect pour le depth/stencil
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        createInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        // Correction : stocker la vue dans depthImageViews au lieu de swapChainImageViews
        if (vkCreateImageView(renderEngine->GetDevice(), &createInfo, nullptr, &depthImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create depth image views!");
        }
    }
}

void VulkanSwapchain::CreateFramebuffer()
{
    framebuffers.resize(imageViews.size());
    for (size_t i = 0; i < imageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = 
        {
            imageViews[i], 
            depthImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = GetExtent().width;
        framebufferInfo.height = GetExtent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(renderEngine->GetDevice(), &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void VulkanSwapchain::CreateDepthResources()
{
    depthImages.resize(images.size());
    depthImageMemorys.resize(images.size());

    for (size_t i = 0; i < images.size(); i++)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = GetExtent().width;
        imageInfo.extent.height = GetExtent().height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = renderEngine->GetDepthStencilImageFormat();
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(renderEngine->GetDevice(), &imageInfo, nullptr, &depthImages[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create depth image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(renderEngine->GetDevice(), depthImages[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = renderEngine->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(renderEngine->GetDevice(), &allocInfo, nullptr, &depthImageMemorys[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate depth image memory!");
        }

        vkBindImageMemory(renderEngine->GetDevice(), depthImages[i], depthImageMemorys[i], 0);
    }
}

void VulkanSwapchain::Cleanup()
{
    // Détruire les framebuffers
    for (auto framebuffer : framebuffers) 
    {
        if (framebuffer != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(renderEngine->GetDevice(), framebuffer, nullptr);
        }
    }
    framebuffers.clear();

    // Détruire les image views
    for (size_t i = 0; i < imageViews.size(); i++) 
    {
        vkDestroyImageView(renderEngine->GetDevice(), imageViews[i], nullptr);
        vkDestroyImageView(renderEngine->GetDevice(), depthImageViews[i], nullptr);
        vkDestroyImage(renderEngine->GetDevice(), depthImages[i], nullptr);
        vkFreeMemory(renderEngine->GetDevice(), depthImageMemorys[i], nullptr);

//        if (inFlightFence[i] != VK_NULL_HANDLE) 
//        {
//            vkDestroyFence(renderEngine->GetDevice(), inFlightFence[i], nullptr);
//            inFlightFence[i] = VK_NULL_HANDLE;
//        }
//        if (renderFinishedSemaphore[i] != VK_NULL_HANDLE) 
//        {
//            vkDestroySemaphore(renderEngine->GetDevice(), renderFinishedSemaphore[i], nullptr);
//            renderFinishedSemaphore[i] = VK_NULL_HANDLE;
//        }
    }

    imageViews.clear();
    depthImageViews.clear();
    depthImages.clear();
    depthImageMemorys.clear();
//    renderFinishedSemaphore.clear();
//    inFlightFence.clear();

    // Détruire la swap chain
    if (swapchain != VK_NULL_HANDLE) 
    {
        vkDestroySwapchainKHR(renderEngine->GetDevice(), swapchain, nullptr);
        swapchain = VK_NULL_HANDLE;
    }
}
