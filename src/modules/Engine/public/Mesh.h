#ifndef MESH_H
#define MESH_H

#include "IVulkanMesh.h"
#include "VulkanUniformBuffer.h"
#include "EngineCore.hpp"

class VulkanRenderEngine;

class Mesh : public IVulkanMesh
{
/* GLOBAL */
public: 
    Mesh() = default;
    Mesh(const Transform& transform) : meshTransform(transform) {}
    ~Mesh();

public: // Global public functions
    const Transform& GetMeshTransform() const { return meshTransform; }
    void SetMeshTransform(Transform newTransform);

private: // Global variables
    Transform meshTransform{};


/* VULKAN */
public: //IVulkanMesh Interface
    void InitVulkanMesh(VulkanRenderEngine* vulkanRenderEngine);

public: // Public vulkan functions

protected: // Protected vulkan functions
    const bool& IsVulkanInitialized() const { return isVulkanInitialized; }

    VulkanRenderEngine* GetVulkanRenderEngine() const { return vulkanRenderEngine; }
    
    VulkanUniformBuffer& GetVulkanMeshBuffer() { return vulkanMeshBuffer; }
    const VulkanUniformBuffer& GetVulkanMeshBuffer() const { return vulkanMeshBuffer; }
    VkDescriptorSet& GetVulkanMeshDescriptorSet() { return vulkanMeshDescriptorSet; } 
    const VkDescriptorSet& GetVulkanMeshDescriptorSet() const { return vulkanMeshDescriptorSet; } 

    virtual void UpdateVulkanMeshBuffer();

private: // Vulkan variables
    bool isVulkanInitialized{false};
    VulkanRenderEngine* vulkanRenderEngine{nullptr};
    VulkanUniformBuffer vulkanMeshBuffer;
    VkDescriptorSet vulkanMeshDescriptorSet{VK_NULL_HANDLE};

};

#endif // MESH_H