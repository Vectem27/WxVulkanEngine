add_library(Logger STATIC
    private/Logger.cpp
)

target_include_directories(Logger 
    PUBLIC public
    PRIVATE private
)