#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "IRenderEngine.h"

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
#include "VulkanCamera.h"
#include "IRenderEngine.h"

class VulkanRenderer : public IRenderEngine
{
public:
    VulkanRenderer() : instance(VK_NULL_HANDLE), physicalDevice(VK_NULL_HANDLE), device(VK_NULL_HANDLE),
                       graphicsQueue(VK_NULL_HANDLE), surface(VK_NULL_HANDLE), graphicsQueueFamilyIndex(0) 
    { }

    ~VulkanRenderer() { Shutdown(); }

    virtual bool Init(void* windowHandle) override;
    void render();
    virtual void Shutdown() override;


    VkDevice GetDevice() const { return device; }
    VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
    VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }


    VkFormat GetSwapChainImageFormat() const { return swapChainImageFormat;}
    VkQueue GetPresentQueue() const { return presentQueue; }
    VkQueue GetGraphicsQueue() const { return graphicsQueue; }

    const VkDescriptorSetLayout* GetCameraDescriptorLayout() const { return &cameraDescriptorLayout; }
    const VkDescriptorSetLayout* GetObjectDescriptorLayout() const { return &objectDescriptorLayout; }
private:
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    // Rendering
    VkFormat swapChainImageFormat;
    SwapchainRenderer* swapchainRenderer;
    VulkanCamera* camera;

    uint32_t graphicsQueueFamilyIndex;


    VkDescriptorSetLayout cameraDescriptorLayout;
    VkDescriptorSetLayout objectDescriptorLayout;
    
public:
    // Material
    VkDescriptorPool descriptorPool;
    Material material;
private:
    void createInstance();
    void createSurface(void* windowHandle);
    void selectPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    void createLogicalDevice();
    void InitMaterials();
    void CreateDescriptorLayouts();
    void createDescriptorPool();
public:
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = usage;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
        if (vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create buffer!");
        }
    
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);
    
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
    
        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }
    
        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }
    
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
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
};

#endif // VULKAN_RENDERER_H
