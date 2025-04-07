add_library(RenderApi INTERFACE interface/IRenderable.h)

target_include_directories(RenderApi 
    INTERFACE interface
)

target_include_directories(RenderApi INTERFACE EngineCore)
target_link_libraries(RenderApi INTERFACE EngineCore)