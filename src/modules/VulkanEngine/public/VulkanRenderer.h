#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include <vector>
#include <stdexcept>
#include <iostream>

struct Vertex 
{
    float pos[2]; // Position en 2D
    float color[3]; // Couleur
};

class VulkanRenderer 
{
public:
    VulkanRenderer() : instance(VK_NULL_HANDLE), physicalDevice(VK_NULL_HANDLE), device(VK_NULL_HANDLE),
                       graphicsQueue(VK_NULL_HANDLE), surface(VK_NULL_HANDLE), swapChain(VK_NULL_HANDLE),
                       renderPass(VK_NULL_HANDLE), commandPool(VK_NULL_HANDLE),
                       imageAvailableSemaphore(VK_NULL_HANDLE), renderFinishedSemaphore(VK_NULL_HANDLE),
                       graphicsQueueFamilyIndex(0) { }

    ~VulkanRenderer() {
        cleanup();
    }

    // Initialise Vulkan en utilisant le handle de la fenêtre native.
    bool init(void* windowHandle);

    // Lance le rendu : ici, on se contente d’un clear en noir.
    void render();

    // Nettoyage de toutes les ressources Vulkan.
    void cleanup();

    void setClearColor(float r, float g, float b, float a);

    float* getClearColor();

private:
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;


    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    uint32_t graphicsQueueFamilyIndex;

    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);



    // Création de l'instance Vulkan.
    void createInstance();

    // Création de la surface en fonction de la plateforme.
    void createSurface(void* windowHandle);

    // Choix du GPU physique.
    void selectPhysicalDevice();

    // Pour cet exemple, on considère qu'un GPU est adapté s'il possède une file graphique
    // avec support de la présentation sur la surface.
    bool isDeviceSuitable(VkPhysicalDevice device);

    // Création du device logique et récupération de la file graphique.
    void createLogicalDevice();

    // Création d'un swapchain avec des réglages de base.
    void createSwapChain();

    // Création des vues d'image pour chaque image du swapchain.
    void createImageViews();

    // Création d'une render pass avec une unique attache couleur.
    void createRenderPass();

    void createGraphicsPipeline();

    // Création des framebuffers pour chaque image vue.
    void createFramebuffers();

    // Création d'une command pool pour allouer les command buffers.
    void createCommandPool();

    // Allocation et enregistrement des command buffers qui se contenteront de lancer un clear.
    void createCommandBuffers();

    // Création de deux sémaphores pour synchroniser l'acquisition et la présentation.
    void createSemaphores();


    void initShaderModule(VkShaderModule *shaderModule, const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    
        if (vkCreateShaderModule(device, &createInfo, nullptr, shaderModule) != VK_SUCCESS) 
        {
            throw std::runtime_error("Échec de la création du module shader !");
        }
    }

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = usage;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
        if (vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }
    
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);
    
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
    
        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }
    
        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }
    
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
    
        // Parcourir tous les types de mémoire pour trouver celui qui correspond aux critères
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
    
        throw std::runtime_error("failed to find suitable memory type!");
    }
    
    
};

#endif // VULKAN_RENDERER_H
