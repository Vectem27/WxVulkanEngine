add_library(Core STATIC
    private/EngineCore.cpp
)

target_include_directories(Core 
    PUBLIC public
    PRIVATE private
)

target_link_libraries(Core
    PUBLIC
        EngineTypes
        Logger
)