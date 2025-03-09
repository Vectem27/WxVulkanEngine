#include "VulkanRenderer.h"
#include <sstream>
#include <array>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

bool VulkanRenderer::init(void *windowHandle, ViewportData viewportSize)
{
    viewportData = viewportSize;
    // 1 Create Instance
    createInstance();

    // 2 Create Surface
    createSurface(windowHandle);

    // 3 Device
    selectPhysicalDevice();

    // 4 Create logical device & Queue
    createLogicalDevice();

    // 5 Create Swapchain
    createSwapChain();

    // 6 Create Image Views
    createImageViews();

    // 7 Create render pass
    createRenderPass();

    // 9 Create Framebuffers
    createFramebuffers();


    // 8 Create Render Pipeline
    createDescriptorPool();
    InitMaterials();


    // 10 Create commande pool & commande buffer
    createCommandPool();
    createCommandBuffers();

    // 11 Create object synchronisation (Semaphore & Fences)
    createSemaphores();


    return true;
}

void VulkanRenderer::render()
{
    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFence);

    uint32_t imageIndex;
    auto resANI = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    if (resANI == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (resANI != VK_SUCCESS && resANI != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("vkAcquireNextImageKHR failed!");
    }

    // Réinitialise le command buffer
    if (vkResetCommandBuffer(commandBuffers[imageIndex], 0) != VK_SUCCESS) {
        throw std::runtime_error("failed to reset command buffer!");
    }

    // Enregistre les commandes de rendu
    recordCommandBuffer(commandBuffers[imageIndex], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    VkResult res = vkQueuePresentKHR(graphicsQueue, &presentInfo);
    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR) {
        recreateSwapChain();
    } else if (res != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }
}

void VulkanRenderer::cleanup() {
    // Attendre que le device soit inactif avant de nettoyer
    if (device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(device);
    }

    // Nettoyer la swap chain
    cleanupSwapChain();

    // Détruire le descriptor pool
    if (descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        descriptorPool = VK_NULL_HANDLE;
    }

    // Détruire les sémaphores et la fence
    if (imageAvailableSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
        imageAvailableSemaphore = VK_NULL_HANDLE;
    }
    if (renderFinishedSemaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
        renderFinishedSemaphore = VK_NULL_HANDLE;
    }
    if (inFlightFence != VK_NULL_HANDLE) {
        vkDestroyFence(device, inFlightFence, nullptr);
        inFlightFence = VK_NULL_HANDLE;
    }

    // Détruire le command pool
    if (commandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(device, commandPool, nullptr);
        commandPool = VK_NULL_HANDLE;
    }

    // Détruire le device logique
    if (device != VK_NULL_HANDLE) {
        vkDestroyDevice(device, nullptr);
        device = VK_NULL_HANDLE;
    }

    // Détruire la surface
    if (surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        surface = VK_NULL_HANDLE;
    }

    // Détruire l'instance Vulkan
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
    }
}

void VulkanRenderer::setClearColor(float r, float g, float b, float a)
{
    clearColor.color.float32[0] = r;
    clearColor.color.float32[1] = g;
    clearColor.color.float32[2] = b;
    clearColor.color.float32[3] = a;
}

float *VulkanRenderer::getClearColor()
{
    return clearColor.color.float32;
}

void VulkanRenderer::ViewportSize(ViewportData viewportData)
{
    this->viewportData = viewportData; 
}

void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    const std::vector<Vertex> vertices =
        {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Vertex 1 (red)
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Vertex 3 (green)
            {{-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}},  // Vertex 2 (purple)

            {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},  // Vertex 1 (blue)
            {{-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}},  // Vertex 2 (purple)
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Vertex 3 (green)
        };

    // Définir la taille du buffer
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    // Créer le buffer de vertex
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

    // Copier les données des vertices dans le buffer
    void *data;
    vkMapMemory(device, vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, vertexBufferMemory);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin command buffer!");
    }
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(viewportData.width);
    viewport.height = static_cast<float>(viewportData.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;


    VkRect2D scissor{}; 
    scissor.offset = {0, 0};
    scissor.extent = {viewportData.width, viewportData.height};

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


    // Débute le render pass
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;                         // Variable de classe
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex]; // Variable de classe
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {viewportData.width, viewportData.height}; // Taille de la fenêtre

    // Couleur de fond (noir)
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    ViewData uboData = {};
    uboData.model = glm::mat4(1.0f);
    float aspectRatio = static_cast<float>(viewportData.width) / static_cast<float>(viewportData.height);
    uboData.proj = glm::perspective(glm::radians(103.0f), aspectRatio, 0.1f, 100.0f);
    uboData.view = glm::lookAt(
        glm::vec3(1.0f, -1.0f, 1.0f), // Position caméra
        glm::vec3(0.0f, 0.0f, 0.0f),  // Point cible
        glm::vec3(0.0f, 1.0f, 0.0f)   // Vecteur "up"
    );

    ubo.update(&uboData);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material.GetGraphicsPipeline());

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material.GetPipelineLayout(), 0, 1, material.GetDescriptorSet(), 0, nullptr);

    // Offset dans vertex des données
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);

    // Dessine un triangle (exemple simple)
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);

    // Termine le render pass
    vkCmdEndRenderPass(commandBuffer);

    // Termine l'enregistrement du command buffer
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to end command buffer!");
    }
}

void VulkanRenderer::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan wxWidgets App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char *> extensions;
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _WIN32
    extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create Vulkan instance!");
    }
}

void VulkanRenderer::createSurface(void *windowHandle)
{
#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = reinterpret_cast<HWND>(windowHandle);
    createInfo.hinstance = GetModuleHandle(nullptr);
    if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
#else
    // Pour d'autres plateformes, il faudra ajouter le code approprié.
#endif
}

void VulkanRenderer::selectPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    for (const auto &device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    bool graphicsFound = false;
    bool presentFound = false;
    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphicsFound = true;
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport)
            {
                presentFound = true;
                break;
            }
        }
    }
    return graphicsFound && presentFound;
}

void VulkanRenderer::createLogicalDevice()
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if (presentSupport)
            {
                graphicsQueueFamilyIndex = i;
                break;
            }
        }
    }
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;

    // Extension nécessaire pour le swapchain.
    const char *deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = deviceExtensions;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }
    vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &presentQueue);
}

void VulkanRenderer::createSwapChain()
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

    VkExtent2D extent = capabilities.currentExtent;
    if (extent.width == UINT32_MAX)
    {
        extent.width = std::clamp(viewportData.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(viewportData.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    }
    swapChainExtent = extent;
    swapChainImageFormat = VK_FORMAT_B8G8R8A8_SRGB;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && createInfo.minImageCount > capabilities.maxImageCount)
    {
        createInfo.minImageCount = capabilities.maxImageCount;
    }
    createInfo.imageFormat = swapChainImageFormat;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = swapChainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
}

void VulkanRenderer::createImageViews()
{
    swapChainImageViews.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views!");
        }
    }
}

void VulkanRenderer::createRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }
}

void VulkanRenderer::InitMaterials()
{
    ubo.create(device, physicalDevice, sizeof(ViewData));

    MaterialInfo matInfo = {};
    matInfo.fragmentShader = "shaders/shader.frag";
    matInfo.vertexShader = "shaders/shader.vert";

    material.Init(this, matInfo);
}

void VulkanRenderer::createFramebuffers()
{
    swapChainFramebuffers.resize(swapChainImageViews.size());
    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        VkImageView attachments[] = {swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void VulkanRenderer::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

void VulkanRenderer::createCommandBuffers()
{
    commandBuffers.resize(swapChainFramebuffers.size());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void VulkanRenderer::createSemaphores()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create semaphores!");
    }

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // La fence est initialement signalée

    if (vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create fence!");
    }
}

void VulkanRenderer::createDescriptorPool()
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 100; // Gérer plusieurs materials

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 100;

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Échec de la création du descriptor pool !");
    }
}

void VulkanRenderer::recreateSwapChain()
{
    vkDeviceWaitIdle(device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createFramebuffers();
    createCommandBuffers();
}

void VulkanRenderer::cleanupSwapChain() {
    // Détruire les framebuffers
    for (auto framebuffer : swapChainFramebuffers) {
        if (framebuffer != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
    }
    swapChainFramebuffers.clear(); // Vider la liste après destruction

    // Détruire le render pass
    if (renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(device, renderPass, nullptr);
        renderPass = VK_NULL_HANDLE;
    }

    // Détruire les image views
    for (auto imageView : swapChainImageViews) {
        if (imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(device, imageView, nullptr);
        }
    }
    swapChainImageViews.clear();

    // Détruire la swap chain
    if (swapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(device, swapChain, nullptr);
        swapChain = VK_NULL_HANDLE;
    }
}
