add_library(VulkanEngine STATIC private/VulkanRenderer.cpp private/Material.cpp private/RenderTarget.cpp private/SwapchainRenderer.cpp)
target_include_directories(VulkanEngine 
    PUBLIC public
    PRIVATE private
)

target_include_directories(VulkanEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanEngine PUBLIC ${Vulkan_LIBRARIES})