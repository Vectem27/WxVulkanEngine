add_library(VulkanEngine STATIC private/VulkanCamera.cpp private/VulkanRenderEngine.cpp private/VulkanMaterial.cpp private/SwapchainRenderer.cpp private/Scene.cpp
    private/VulkanSurface.cpp private/VulkanRenderTarget.cpp private/VulkanSwapchain.cpp private/VulkanRenderer.cpp)
target_include_directories(VulkanEngine 
    PUBLIC public
    PRIVATE private
)

target_include_directories(VulkanEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanEngine PUBLIC ${Vulkan_LIBRARIES} RenderApi)