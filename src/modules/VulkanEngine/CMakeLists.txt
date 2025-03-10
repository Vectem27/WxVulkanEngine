add_library(VulkanEngine STATIC private/VulkanRenderer.cpp private/Material.cpp private/SwapchainRenderer.cpp, private/VulkanCamera.cpp)
target_include_directories(VulkanEngine 
    PUBLIC public
    PRIVATE private
)

target_include_directories(VulkanEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanEngine PUBLIC ${Vulkan_LIBRARIES} RenderApi)