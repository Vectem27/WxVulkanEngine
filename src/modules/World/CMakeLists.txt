add_library(World STATIC 
    private/World.cpp
    private/SceneComponent.cpp
    private/ActorBase.cpp
    private/Actor.cpp
)

target_include_directories(World 
    PUBLIC public
    PRIVATE private
)

target_link_libraries(World PUBLIC EngineCore RenderApi)
