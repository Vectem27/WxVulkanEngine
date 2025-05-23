file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(VulkanAPI STATIC 
    ${SRC_FILES}
    ${HEADER_FILES}
)


target_include_directories(VulkanAPI 
    PUBLIC public
    PRIVATE private

    PUBLIC public/Objects/Light
    PRIVATE private/Objects/Light

    PUBLIC public/Pipeline
    PRIVATE private/Pipeline
    
    PUBLIC public/Helper/Descriptor
    PRIVATE private/Helper/Descriptor
    PUBLIC public/Helper/Command
    PRIVATE private/Helper/Command
    PUBLIC public/Helper/Image
    PRIVATE private/Helper/Image
    PUBLIC public/Helper/Pipeline
    PRIVATE private/Helper/Pipeline

    PUBLIC public/Buffer
    PRIVATE private/Buffer

    PUBLIC public/Objects/Texture
    PRIVATE private/Objects/Texture

    PUBLIC public/Objects/Material
    PRIVATE private/Objects/Material

    PUBLIC public/Objects/Mesh
    PRIVATE private/Objects/Mesh

    PUBLIC public/Rendering
    PRIVATE private/Rendering

    PUBLIC public/Objects
    PRIVATE private/Objects

    PUBLIC public/Objects/Camera
    PRIVATE private/Objects/Camera
)

target_compile_features(VulkanAPI PRIVATE cxx_std_20)

target_include_directories(VulkanAPI PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanAPI PUBLIC ${Vulkan_LIBRARIES} EngineCore)