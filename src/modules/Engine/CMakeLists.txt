file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(Engine STATIC 
    ${SRC_FILES}
    ${HEADER_FILES}
)

target_include_directories(Engine 
    PUBLIC public
    PRIVATE private

    PUBLIC public/SceneRenderer
    PRIVATE private/SceneRenderer
)

target_include_directories(Engine PUBLIC World VulkanEngine)
target_link_libraries(Engine PUBLIC World VulkanEngine)