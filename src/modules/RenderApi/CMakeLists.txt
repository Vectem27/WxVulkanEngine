add_library(RenderApi INTERFACE public/IRenderable.h)

target_include_directories(RenderApi 
    INTERFACE public
)

target_include_directories(RenderApi INTERFACE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(RenderApi INTERFACE ${Vulkan_LIBRARIES} RenderApi)