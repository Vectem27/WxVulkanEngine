#include "VulkanRenderer.h"
#include <sstream>
#include <array>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

bool VulkanRenderer::Init(void *windowHandle)
{
    swapChainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

    // 1 Create Instance
    createInstance();

    // 2 Create Surface
    createSurface(windowHandle);

    // 3 Device
    selectPhysicalDevice();

    // 4 Create logical device & Queue
    createLogicalDevice();

    createRenderPass();

    swapchainRenderer = new SwapchainRenderer(device, physicalDevice, surface, renderPass, swapChainImageFormat, graphicsQueueFamilyIndex);

    // 8 Create Render Pipeline
    createDescriptorPool();
    InitMaterials();

    return true;
}

void VulkanRenderer::render()
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

    auto cmd = swapchainRenderer->BeginRenderCommands(&clearColor);

    if(cmd == VK_NULL_HANDLE)
    {
        std::cout << "nullhandle" << std::endl;
    }

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapchainRenderer->width);
    viewport.height = static_cast<float>(swapchainRenderer->height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;


    VkRect2D scissor{}; 
    scissor.offset = {0, 0};
    scissor.extent = {swapchainRenderer->width, swapchainRenderer->height};

    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);



    ViewData uboData = {};
    uboData.model = glm::mat4(1.0f);
    float aspectRatio = static_cast<float>(swapchainRenderer->width) / static_cast<float>(swapchainRenderer->height);
    uboData.proj = glm::perspective(glm::radians(103.0f), aspectRatio, 0.1f, 100.0f);
    uboData.view = glm::lookAt(
        glm::vec3(1.0f, -1.0f, 1.0f), // Position caméra
        glm::vec3(0.0f, 0.0f, 0.0f),  // Point cible
        glm::vec3(0.0f, 1.0f, 0.0f)   // Vecteur "up"
    );

    ubo.update(&uboData);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, material.GetGraphicsPipeline());

    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, material.GetPipelineLayout(), 0, 1, material.GetDescriptorSet(), 0, nullptr);

    // Offset dans vertex des données
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffer, &offset);

    // Dessine un triangle (exemple simple)
    vkCmdDraw(cmd, 6, 1, 0, 0);

    swapchainRenderer->EndRenderCommandsAndPresent(graphicsQueue);
}

void VulkanRenderer::Shutdown() {
    // Attendre que le device soit inactif avant de nettoyer
    if (device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(device);
    }

    // Nettoyer la swap chain
    delete swapchainRenderer;
    
    // Détruire le render pass
    if (renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(device, renderPass, nullptr);
        renderPass = VK_NULL_HANDLE;
    }

    // Détruire le descriptor pool
    if (descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        descriptorPool = VK_NULL_HANDLE;
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
#elif defined(__linux__)  // Pour Linux
    VkXcbSurfaceCreateInfoKHR createInfo{}; 
    createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfo.connection = XGetXCBConnection(XOpenDisplay(nullptr));
    createInfo.window = reinterpret_cast<xcb_window_t>(windowHandle);
    if (vkCreateXcbSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create XCB window surface!");
    }
#elif defined(__APPLE__)  // Pour macOS
    VkCocoaSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COCOA_SURFACE_CREATE_INFO_KHR;
    createInfo.pView = windowHandle;  // Utilisation du handle natif macOS
    if (vkCreateCocoaSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create Cocoa window surface!");
    }
#else
    // Autres plateformes
    throw std::runtime_error("Surface creation not supported for this platform!");
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