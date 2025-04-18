add_subdirectory(Logger)
add_subdirectory(Types)
add_subdirectory(Handling)
add_subdirectory(Core)


add_library(EngineCore INTERFACE)


target_link_libraries(EngineCore
    INTERFACE
        Handling
        EngineTypes
        Logger
        Core
)