#include "VulkanSwapchain.h"
#include "VulkanRenderEngine.h"
#include <algorithm>
#include <array>

#include "VulkanRenderPassManager.h"
#include "VulkanRenderImageManager.h"
#include "VulkanDescriptorPoolBuilder.h"

#include "VulkanDescriptorUtils.h"

VulkanSwapchain::VulkanSwapchain(VulkanRenderEngine *renderEngine, VulkanSurface* surface)
    : renderEngine(renderEngine), surface(surface)
{ 
    VulkanDescriptorPoolBuilder poolBuilder;
    descriptorPool = poolBuilder.SetMaxSets(5).AddCombinedImageSampler(5).Build();

    VkResult result;
    auto gBufferSetLayout = renderEngine->GetPipelineManager()->GetgBufferDescriptorSetLayout();
    result = VulkanDescriptorUtils::AllocateSet(descriptorPool, gBufferSetLayout, gBufferDescriptorSet);
    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to allocate swapchain gBuffer descriptor set");
        throw std::runtime_error("Failed to allocate swapchain gBuffer descriptor set");
    }

    auto postprocessSetLayout = renderEngine->GetPipelineManager()->GetPostprocessDescriptorSetLayout();
    result = VulkanDescriptorUtils::AllocateSet(descriptorPool, postprocessSetLayout, postprocessDescriptorSet);
    if (result != VK_SUCCESS)
    {
        Log(Error, "Vulkan", "Failed to allocate swapchain post-process descriptor set");
        throw std::runtime_error("Failed to allocate swapchain post-process descriptor set");
    }

    CreateCommandPool(renderEngine->GetDeviceManager()->GetGraphicsQueueFamilyIndex());
    CreateCommandBuffers();
    CreateSync();
    CreateSwapchain();
}


void VulkanSwapchain::Create(VkRenderPass renderPass)
{
    CreateSwapchain();
}

void VulkanSwapchain::Recreate()
{
    vkDeviceWaitIdle(GetVulkanDeviceManager().GetDeviceChecked());
    Cleanup();
    CreateSwapchain();
}

void VulkanSwapchain::Present()
{
    static uint8_t infiniteLoopGuard{0};
    const uint8_t maxLoopCount{3};

    if (infiniteLoopGuard > maxLoopCount)
    {
        Log(Error, "Vulkan", "Failed present swapchain : infinite loop detected");
        throw std::runtime_error("Failed to present swapchain : infinite loop detected");
    }

    ++infiniteLoopGuard;

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &GetRenderFinishedSemaphore();
    presentInfo.swapchainCount = 1;
    
    VkSwapchainKHR swapChains[] = {GetSwapchain()};
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &renderingImageIndex;

    VkResult res = vkQueuePresentKHR(surface->GetPresentQueue(), &presentInfo);
    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) 
    {
        Log(Trace, "Vulkan", "Can't present swapchain, recreate called");
        Recreate();
        Present();
    }
    else if (res != VK_SUCCESS) 
    {
        infiniteLoopGuard = 0;
        Log(Error, "Vulkan", "Failed to present swapchain, result code : %d", res);
        throw std::runtime_error("Failed to present swapchain");
    }

    infiniteLoopGuard = 0;
}

void VulkanSwapchain::CreateSwapchain()
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(GetVulkanDeviceManager().GetPhysicalDeviceChecked(), surface->GetSurface(), &capabilities);

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

    createInfo.imageFormat = GetVulkanRenderPassManager().GetColorFormat();
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

    if (vkCreateSwapchainKHR(GetVulkanDeviceManager().GetDeviceChecked(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
        throw std::runtime_error("Failed to create swap chain!");

    vkGetSwapchainImagesKHR(GetVulkanDeviceManager().GetDeviceChecked(), swapchain, &imageCount, nullptr);

    swapchainImages.resize(imageCount);
    swapchainImageViews.resize(imageCount);
    renderTargets.resize(imageCount);

    vkGetSwapchainImagesKHR(GetVulkanDeviceManager().GetDeviceChecked(), swapchain, &imageCount, swapchainImages.data());

    
    for (int i = 0; i < imageCount; ++i)
    {
        renderTargets[i].Init(GetWidth(), GetHeight());

        GetVulkanImageManager().CreateImageView(
            swapchainImageViews[i],
            swapchainImages[i],
            VulkanRenderPassManager::GetInstance()->GetColorFormat(),
            VK_IMAGE_ASPECT_COLOR_BIT
        );

        renderTargets[i].GetPostprocessImageView() = swapchainImageViews[i];
        renderTargets[i].CreateFrameBuffer();
    }
}

void VulkanSwapchain::CreateSync()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(GetVulkanDeviceManager().GetDeviceChecked(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(GetVulkanDeviceManager().GetDeviceChecked(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // La fence est initialement signalée
    if (vkCreateFence(GetVulkanDeviceManager().GetDeviceChecked(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
        throw std::runtime_error("Failed to create inFlight fence!");
}

void VulkanSwapchain::UpdateGBufferDescriptorSet()
{
    // Préparez les informations d'image pour chaque attachement
    VkDescriptorImageInfo positionInfo = {
        .sampler = renderEngine->GetPipelineManager()->GetGBufferSampler(),
        //.imageView = GetImagesData(BufferType::POSITION).imageViews[renderingImageIndex], // Vue de votre texture de position
        .imageView = renderTargets[renderingImageIndex].GetPositionImageView(), // Vue de votre texture de position
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    VkDescriptorImageInfo normalInfo = {
        .sampler = renderEngine->GetPipelineManager()->GetGBufferSampler(),
        //.imageView = GetImagesData(BufferType::NORMAL).imageViews[renderingImageIndex], // Vue de votre texture de normales
        .imageView = renderTargets[renderingImageIndex].GetNormalImageView(), // Vue de votre texture de position
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    VkDescriptorImageInfo baseColorInfo = {
        .sampler = renderEngine->GetPipelineManager()->GetGBufferSampler(),
        //.imageView = GetImagesData(BufferType::BASECOLOR).imageViews[renderingImageIndex], // Vue de votre texture d'albedo
        .imageView = renderTargets[renderingImageIndex].GetBaseColorImageView(), // Vue de votre texture de position
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    // Configurez les écritures pour le descriptor set
    VkWriteDescriptorSet descriptorWrites[3] = {
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = gBufferDescriptorSet,
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &positionInfo
        },
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = gBufferDescriptorSet,
            .dstBinding = 1,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &normalInfo
        },
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = gBufferDescriptorSet,
            .dstBinding = 2,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &baseColorInfo
        }
    };

    vkUpdateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), 3, descriptorWrites, 0, nullptr);
}

void VulkanSwapchain::UpdatePostprocessDescriptorSet()
{
    VkDescriptorImageInfo colorInfo = {
        .sampler = renderEngine->GetPipelineManager()->GetGBufferSampler(),
        //.imageView = GetImagesData(BufferType::BASECOLOR).imageViews[renderingImageIndex], // Vue de votre texture de position
        .imageView = renderTargets[renderingImageIndex].GetBaseColorImageView(), // Vue de votre texture de position
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    VkDescriptorImageInfo lightingInfo = {
        .sampler = renderEngine->GetPipelineManager()->GetGBufferSampler(),
        //.imageView = GetImagesData(BufferType::LIGHTING).imageViews[renderingImageIndex], // Vue de votre texture de position
        .imageView = renderTargets[renderingImageIndex].GetLightingImageView(), // Vue de votre texture de position
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    // Configurez les écritures pour le descriptor set
    VkWriteDescriptorSet descriptorWrites[2] = {
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = postprocessDescriptorSet,
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &colorInfo
        },
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = postprocessDescriptorSet,
            .dstBinding = 1,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &lightingInfo
        }
    };

    vkUpdateDescriptorSets(GetVulkanDeviceManager().GetDeviceChecked(), 2, descriptorWrites, 0, nullptr);
}

void VulkanSwapchain::CreateCommandPool(uint32_t graphicsQueueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    if (vkCreateCommandPool(GetVulkanDeviceManager().GetDeviceChecked(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create swapchain command pool!");
}

void VulkanSwapchain::CreateCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(GetVulkanDeviceManager().GetDeviceChecked(), &allocInfo, &commandBuffer) != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate swapchain command buffers!");
}

void VulkanSwapchain::Cleanup()
{
    for (auto& rt : renderTargets)
        rt.Cleanup();

    renderTargets.clear();

    swapchainImageViews.clear();

    if (swapchain != VK_NULL_HANDLE) 
    {
        vkDestroySwapchainKHR(GetVulkanDeviceManager().GetDeviceChecked(), swapchain, nullptr);
        swapchain = VK_NULL_HANDLE;
    }

    swapchainImages.clear();
}

VulkanSwapchain::~VulkanSwapchain()
{
    Cleanup();

    auto device = GetVulkanDeviceManager().GetDeviceChecked();

    if (descriptorPool != VK_NULL_HANDLE)
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);   

    // Détruire les sémaphores et la fence
    if (imageAvailableSemaphore != VK_NULL_HANDLE) 
        vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);

    if (renderFinishedSemaphore != VK_NULL_HANDLE) 
        vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);

    if (inFlightFence != VK_NULL_HANDLE) 
        vkDestroyFence(device, inFlightFence, nullptr);

    if (commandPool != VK_NULL_HANDLE) 
        vkDestroyCommandPool(device, commandPool, nullptr);
}

void VulkanSwapchain::StartRendering()
{
    static uint8_t infiniteLoopGuard{0};
    const uint8_t maxLoopCount{3};

    if (infiniteLoopGuard > maxLoopCount)
    {
        Log(Error, "Vulkan", "Failed to start swapchain rendering : infinite loop detected");
        throw std::runtime_error("Failed to start swapchain rendering : infinite loop detected");
    }

    ++infiniteLoopGuard;

    auto res = vkAcquireNextImageKHR(GetVulkanDeviceManager().GetDeviceChecked(), GetSwapchain(), UINT64_MAX, GetImageAvailableSemaphore(), VK_NULL_HANDLE, &renderingImageIndex);
    if (res == VK_ERROR_OUT_OF_DATE_KHR) 
    {
        Log(Trace, "Vulkan", "Can't start swapchain rendering, recreate called");
        Recreate();
        StartRendering();
    } 
    else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) 
    {
        infiniteLoopGuard = 0;
        Log(Error, "Vulkan", "Failed to acquire next swapchain image, result code : %d", res);
        throw std::runtime_error("Failed to acquire next swapchain image");
    }
    infiniteLoopGuard = 0;
}
