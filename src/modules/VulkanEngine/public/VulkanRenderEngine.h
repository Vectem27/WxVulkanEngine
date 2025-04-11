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

#include "IRenderEngine.h"
#include "VulkanDeviceManager.h"
#include "VulkanSurface.h"

#include "VulkanDescriptorPoolManager.h"
#include "Pipeline/VulkanPipelineManager.h"

class VulkanRenderEngine : public IRenderEngine
{
public:
    ~VulkanRenderEngine() { Shutdown(); }

    virtual bool Init(void* windowHandle) override;
    void render();
    virtual void Shutdown() override;

    VkInstance GetInstance() { return instance; }

    VkQueue GetPresentQueue() const { return surfaceTest->GetPresentQueue(); }
    //[[deprecated]]
    VkQueue GetGraphicsQueue() const { return GetVulkanDeviceManager().GetGraphicsQueue(); }
    
    VulkanSurface* GetSurfaceTest() const { return surfaceTest; }
    //[[deprecated]]
    VulkanDeviceManager* GetDeviceManager() const { return &GetVulkanDeviceManager(); }
    VulkanDescriptorPoolManager* GetDescriptorPoolManager() { return descriptorPoolManager;}

    VulkanPipelineManager* GetPipelineManager() { return &VulkanPipelineManager::GetInstance(); }
private:
    VkInstance instance{ VK_NULL_HANDLE };
    VulkanSurface* surfaceTest{ nullptr };
    
    VulkanDescriptorPoolManager* descriptorPoolManager;

    VulkanPipelineManager* pipelineManager;
private:
    void createInstance();
    void createDescriptorPool();
public:
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = usage;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
        if (vkCreateBuffer(GetVulkanDeviceManager().GetDeviceChecked(), &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create buffer!");
        }
    
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(GetVulkanDeviceManager().GetDeviceChecked(), buffer, &memRequirements);
    
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
    
        if (vkAllocateMemory(GetVulkanDeviceManager().GetDeviceChecked(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }
    
        vkBindBufferMemory(GetVulkanDeviceManager().GetDeviceChecked(), buffer, bufferMemory, 0);
    }
    
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(GetVulkanDeviceManager().GetPhysicalDeviceChecked(), &memProperties);
    
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
