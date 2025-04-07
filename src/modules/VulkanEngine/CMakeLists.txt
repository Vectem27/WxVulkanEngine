add_library(VulkanEngine STATIC 
    private/VulkanCamera.cpp 
    private/VulkanRenderEngine.cpp
    private/VulkanSurface.cpp 
    private/VulkanRenderTarget.cpp 
    private/VulkanSwapchain.cpp 
    private/VulkanRenderer.cpp
    private/VulkanDeviceManager.cpp 
    private/VulkanSurface.cpp
    private/VulkanDescriptorPoolManager.cpp
    private/Pipeline/VulkanPipelineManager.cpp
    private/Pipeline/VulkanOpaqueMaterial.cpp
    private/VulkanLight.cpp
    
    interface/IVulkanMesh.h
    interface/IVulkanMaterial.h
)


target_include_directories(VulkanEngine 
    PUBLIC public
    INTERFACE interface
    PRIVATE private
)

target_include_directories(VulkanEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanEngine PUBLIC ${Vulkan_LIBRARIES} RenderApi EngineCore)