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
    private/VulkanRenderPassManager.cpp
    private/VulkanRenderImageManager.cpp
    private/Pipeline/PipelineUtils.h
    private/Pipeline/VulkanPipelineManager.cpp
    private/Pipeline/VulkanOpaqueMaterial.cpp

    private/Pipeline/VulkanSpotlightLightPipeline.cpp
    
    public/IVulkanMesh.h
    #public/IVulkanMaterial.h
    public/Light/VulkanLight.h
    private/Light/VulkanSpotlightLight.cpp
    private/Light/VulkanGlobalLightManager.cpp
    private/Light/LightManagers/VulkanSpotlightLightManager.cpp
)


target_include_directories(VulkanEngine 
    INTERFACE interface
    PUBLIC public
    PRIVATE private

    PUBLIC public/Light
    PRIVATE private/Light

    PUBLIC public/Pipeline
    PRIVATE private/Pipeline
)

target_include_directories(VulkanEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanEngine PUBLIC ${Vulkan_LIBRARIES} RenderApi EngineCore)