#include "VulkanSwapchain.h"
#include "VulkanRenderEngine.h"
#include <algorithm>
#include <array>

#include "VulkanRenderPassManager.h"
#include "VulkanRenderImageManager.h"

VulkanSwapchain::VulkanSwapchain(VulkanRenderEngine *renderEngine, VulkanSurface* surface)
    : renderEngine(renderEngine), surface(surface)
{ 
    imagesData.push_back({
        BufferType::BASECOLOR, VulkanRenderPassManager::GetInstance()->GetColorFormat(), 
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    });

    imagesData.push_back({
        BufferType::DEPTHSTENCIL, VulkanRenderPassManager::GetInstance()->GetDepthStencilFormat(),
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT| VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
    });
    imagesData.push_back({
        BufferType::NORMAL, VulkanRenderPassManager::GetInstance()->GetColorFormat(),
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    });

    imagesData.push_back({
        BufferType::LIGHTING, VulkanRenderPassManager::GetInstance()->GetHDRFormat(),
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    });

    imagesData.push_back({
        BufferType::POSITION, VulkanRenderPassManager::GetInstance()->GetHDRFormat(),
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    });

    CreateCommandPool(renderEngine->GetDeviceManager()->GetGraphicsQueueFamilyIndex());
    CreateSync();
    CreateSwapchain();
}


void VulkanSwapchain::Create(VkRenderPass renderPass)
{
    this->renderPass = renderPass;
    CreateSwapchain();
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
    CreateFramebuffer();
    CreateCommandBuffers();
}

void VulkanSwapchain::CreateSwapchain()
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderEngine->GetPhysicalDevice(), surface->GetSurface(), &capabilities);

    VkExtent2D extent = capabilities.currentExtent;
    if (extent.width == UINT32_MAX)
    {
        extent.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }
    width = extent.width;
    height = extent.height;

    swapchainExtent = extent;

    bool queuesAreDifferent = (renderEngine->GetDeviceManager()->GetGraphicsQueueFamilyIndex() != surface->GetPresentQueueFamilyIndex());

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->GetSurface();
    createInfo.minImageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && createInfo.minImageCount > capabilities.maxImageCount)
    {
        createInfo.minImageCount = capabilities.maxImageCount;
    }

    createInfo.imageFormat = GetImagesData(viewBufferType).format;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = swapchainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (queuesAreDifferent) 
    {
        uint32_t queueFamilyIndices[] = {renderEngine->GetDeviceManager()->GetGraphicsQueueFamilyIndex(), surface->GetPresentQueueFamilyIndex()};
        
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;  // Permet à plusieurs queues d'accéder aux images
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } 
    else 
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;  // Plus performant si une seule famille utilise la swapchain
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }
    
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(renderEngine->GetDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
        throw std::runtime_error("Failed to create swap chain!");

    vkGetSwapchainImagesKHR(renderEngine->GetDevice(), swapchain, &imageCount, nullptr);

    for (auto& imageData : imagesData)
        imageData.Resize(imageCount);

    vkGetSwapchainImagesKHR(renderEngine->GetDevice(), swapchain, &imageCount, GetImagesData(viewBufferType).images.data());

    
    for (int i = 0; i < imageCount; ++i)
    {
        for (auto& imageData : imagesData)
        {
            if (viewBufferType != imageData.type)
            {
                VulkanRenderImageManager::GetInstance()->CreateImage(
                    imageData.images[i], imageData.imageMemorys[i], 
                    GetWidth(), GetHeight(), imageData.format,
                    imageData.usageFlags
                );
            }

            VulkanRenderImageManager::GetInstance()->CreateImageView(
                imageData.imageViews[i], imageData.images[i], imageData.format,
                imageData.aspectFlags
            );
        }
    }
}


void VulkanSwapchain::CreateFramebuffer()
{
    framebuffers.resize(imageCount);
    lightingFramebuffers.resize(imageCount);
    for (size_t i = 0; i < imageCount; i++)
    {
        std::vector<VkImageView> attachments = 
        {
            GetImagesData(BufferType::BASECOLOR).imageViews[i], 
            GetImagesData(BufferType::DEPTHSTENCIL).imageViews[i],
            GetImagesData(BufferType::NORMAL).imageViews[i],
            GetImagesData(BufferType::POSITION).imageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = GetExtent().width;
        framebufferInfo.height = GetExtent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(renderEngine->GetDevice(), &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
            throw std::runtime_error("Failed to create swapchain framebuffer!");



        std::vector<VkImageView> lightingAttachments = 
        {
            GetImagesData(BufferType::LIGHTING).imageViews[i]
        };

        framebufferInfo={};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = VulkanRenderPassManager::GetInstance()->GetLightingPass();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(lightingAttachments.size());
        framebufferInfo.pAttachments = lightingAttachments.data();
        framebufferInfo.width = GetExtent().width;
        framebufferInfo.height = GetExtent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(renderEngine->GetDevice(), &framebufferInfo, nullptr, &lightingFramebuffers[i]) != VK_SUCCESS)
            throw std::runtime_error("Failed to create swapchain framebuffer!");
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
        throw std::runtime_error("Failed to create inFlight fence!");
}

void VulkanSwapchain::CreateCommandPool(uint32_t graphicsQueueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    if (vkCreateCommandPool(renderEngine->GetDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create swapchain command pool!");
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
        throw std::runtime_error("Failed to allocate swapchain command buffers!");
}

void VulkanSwapchain::Cleanup()
{
    // Détruire les framebuffers
    for (auto& framebuffer : framebuffers) 
    {
        if (framebuffer != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(renderEngine->GetDevice(), framebuffer, nullptr);
        }
    }
    framebuffers.clear();

    for (auto& framebuffer : lightingFramebuffers) 
    {
        if (framebuffer != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(renderEngine->GetDevice(), framebuffer, nullptr);
        }
    }
    lightingFramebuffers.clear();

    for (auto& imageData : imagesData)
        imageData.Cleanup(renderEngine->GetDevice(), imageData.type == viewBufferType);

    if (swapchain != VK_NULL_HANDLE) 
    {
        vkDestroySwapchainKHR(renderEngine->GetDevice(), swapchain, nullptr);
        swapchain = VK_NULL_HANDLE;
    }
}

VulkanSwapchain::~VulkanSwapchain()
{
    Cleanup();
    
    // Détruire les sémaphores et la fence
    if (imageAvailableSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(renderEngine->GetDevice(), imageAvailableSemaphore, nullptr);
        imageAvailableSemaphore = VK_NULL_HANDLE;
    }

    if (renderFinishedSemaphore != VK_NULL_HANDLE) 
    {
        vkDestroySemaphore(renderEngine->GetDevice(), renderFinishedSemaphore, nullptr);
        renderFinishedSemaphore = VK_NULL_HANDLE;
    }

    if (inFlightFence != VK_NULL_HANDLE) 
    {
        vkDestroyFence(renderEngine->GetDevice(), inFlightFence, nullptr);
        inFlightFence = VK_NULL_HANDLE;
    }

    if (commandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(renderEngine->GetDevice(), commandPool, nullptr);
        commandPool = VK_NULL_HANDLE;
    }
}

