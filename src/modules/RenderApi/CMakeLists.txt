add_library(RenderApi INTERFACE public/IRenderable.h)

target_include_directories(RenderApi 
    INTERFACE public
)

target_include_directories(RenderApi INTERFACE EngineCore ${Vulkan_INCLUDE_DIRS})
target_link_libraries(RenderApi INTERFACE EngineCore ${Vulkan_LIBRARIES})