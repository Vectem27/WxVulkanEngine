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

#include "Material.h"
#include "Vertex.h"
#include "UniformBufferObject.h"

#include "SwapchainRenderer.h"

class VulkanRenderer 
{
public:
    VulkanRenderer() : instance(VK_NULL_HANDLE), physicalDevice(VK_NULL_HANDLE), device(VK_NULL_HANDLE),
                       graphicsQueue(VK_NULL_HANDLE), surface(VK_NULL_HANDLE),
                       renderPass(VK_NULL_HANDLE),
                       graphicsQueueFamilyIndex(0) { }

    ~VulkanRenderer() 
    {
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

    VkDevice GetDevice() const { return device; }
    VkRenderPass GetRenderPass() const { return renderPass; }
    VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }
private:
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkRenderPass renderPass;

    // Rendering
    VkFormat swapChainImageFormat;
    SwapchainRenderer* swapchainRenderer;

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

    uint32_t graphicsQueueFamilyIndex;

    // Material
    VkDescriptorPool descriptorPool;
    Material material;
public:
    UniformBuffer ubo;
private:
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

    void createRenderPass();

    void InitMaterials();

    void createDescriptorPool();

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
