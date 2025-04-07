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
    
    public/IVulkanMesh.h
    #public/IVulkanMaterial.h
    public/Light/VulkanLight.h
    private/Light/VulkanProjectorLight.cpp
    private/Light/VulkanLightManager.cpp
)


target_include_directories(VulkanEngine 
    INTERFACE interface
    PUBLIC public
    PRIVATE private

    PUBLIC public/Light
    PRIVATE private/Light
)

target_include_directories(VulkanEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanEngine PUBLIC ${Vulkan_LIBRARIES} RenderApi EngineCore)