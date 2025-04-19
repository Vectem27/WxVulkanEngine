file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(Engine STATIC 
    ${SRC_FILES}
    ${HEADER_FILES}
)

target_include_directories(Engine 
    PUBLIC public
    PRIVATE private

    PUBLIC public/Rendering
    PRIVATE private/Rendering

    PUBLIC public/PrimaryObjects
    PRIVATE private/PrimaryObjects

    PUBLIC public/Objects
    PRIVATE private/Objects

    PUBLIC public/Components
    PRIVATE private/Components

    PUBLIC public/Actors
    PRIVATE private/Actors

    PUBLIC public/Engine
    PRIVATE private/Engine
)

target_include_directories(Engine PUBLIC World VulkanAPI)
target_link_libraries(Engine PUBLIC World VulkanAPI Rendering)