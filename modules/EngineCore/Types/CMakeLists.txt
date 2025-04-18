add_library(EngineTypes STATIC
    private/Quaternion.cpp
    
    public/Array.h
    public/BoundingBox.h
    public/Converter.hpp
)

target_include_directories(EngineTypes 
    INTERFACE interface
    PUBLIC public
    PRIVATE private
)

target_link_libraries(EngineTypes
    PRIVATE
        Logger
)