# RHI (Rendering Hardware Interface) module
add_subdirectory(RHI)

#--Renderning Backend Implementations--

# Vulkan Rendering module
add_subdirectory(VulkanRendering)


#--Engine Rendering Module--
add_subdirectory(EngineRendering)


add_library(Rendering INTERFACE)

target_link_libraries(
    Rendering INTERFACE
        RHI
        VulkanRendering
        EngineRendering
)