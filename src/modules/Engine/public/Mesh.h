#ifndef MESH_H
#define MESH_H

#include "IVulkanMesh.h"
#include "UniformBufferObject.h"
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
    virtual void InitVulkanMesh(VulkanRenderEngine* vulkanRenderEngine) override;

public: // Public vulkan functions

protected: // Protected vulkan functions
    const bool& IsVulkanInitialized() const { return isVulkanInitialized; }

    VulkanRenderEngine* GetVulkanRenderEngine() const { return vulkanRenderEngine; }
    
    UniformBuffer& GetVulkanMeshBuffer() { return vulkanMeshBuffer; }
    const UniformBuffer& GetVulkanMeshBuffer() const { return vulkanMeshBuffer; }
    VkDescriptorSet& GetVulkanMeshDescriptorSet() { return vulkanMeshDescriptorSet; } 
    const VkDescriptorSet& GetVulkanMeshDescriptorSet() const { return vulkanMeshDescriptorSet; } 

    virtual void UpdateVulkanMeshBuffer();

private: // Vulkan variables
    bool isVulkanInitialized{false};
    VulkanRenderEngine* vulkanRenderEngine{nullptr};
    UniformBuffer vulkanMeshBuffer;
    VkDescriptorSet vulkanMeshDescriptorSet{VK_NULL_HANDLE};

};

#endif // MESH_H