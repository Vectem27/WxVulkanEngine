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

#include "VulkanMaterial.h"
#include "Vertex.h"
#include "UniformBufferObject.h"
#include "IRenderEngine.h"
#include "VulkanDeviceManager.h"
#include "VulkanSurface.h"

class VulkanRenderEngine : public IRenderEngine
{
public:
    ~VulkanRenderEngine() { Shutdown(); }

    virtual bool Init(void* windowHandle) override;
    void render();
    virtual void Shutdown() override;

    VkInstance GetInstance() { return instance; }
    VkDevice GetDevice() const { return deviceManager->GetDevice(); }
    VkPhysicalDevice GetPhysicalDevice() const { return deviceManager->GetPhysicalDevice(); }
    VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }


    VkFormat GetSwapChainImageFormat() const { return swapChainImageFormat;}
    VkFormat GetDepthStencilImageFormat() const { return depthStencilImageFormat;}
    VkQueue GetPresentQueue() const { return surfaceTest->GetPresentQueue(); }
    VkQueue GetGraphicsQueue() const { return deviceManager->GetGraphicsQueue(); }
    VkRenderPass GetDefaultRenderPass() const { return defaultRenderPass; }

    const VkDescriptorSetLayout* GetCameraDescriptorLayout() const { return &cameraDescriptorLayout; }
    const VkDescriptorSetLayout* GetObjectDescriptorLayout() const { return &objectDescriptorLayout; }

    VulkanSurface* GetSurfaceTest() const { return surfaceTest; }
    VulkanDeviceManager* GetDeviceManager() const { return deviceManager; }
private:
    VkInstance instance{ VK_NULL_HANDLE };
    VulkanDeviceManager* deviceManager{ nullptr };
    VulkanSurface* surfaceTest{ nullptr };

    // Rendering
    VkFormat swapChainImageFormat;
    VkFormat depthStencilImageFormat;

    VkDescriptorSetLayout cameraDescriptorLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout objectDescriptorLayout{VK_NULL_HANDLE};

    VkRenderPass defaultRenderPass{VK_NULL_HANDLE};
    
public:
    // Material
    VkDescriptorPool descriptorPool;
    VulkanMaterial material;

private:
    void createInstance();
    void CreateDescriptorLayouts();
    void createDescriptorPool();
    void CreateRenderPass();
public:
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = usage;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
        if (vkCreateBuffer(GetDevice(), &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create buffer!");
        }
    
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(GetDevice(), buffer, &memRequirements);
    
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
    
        if (vkAllocateMemory(GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }
    
        vkBindBufferMemory(GetDevice(), buffer, bufferMemory, 0);
    }
    
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(GetPhysicalDevice(), &memProperties);
    
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
