file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(VulkanEngine STATIC 
    ${SRC_FILES}
    ${HEADER_FILES}

    public/Material/IVulkanMaterial.h
)


target_include_directories(VulkanEngine 
    INTERFACE interface
    PUBLIC public
    PRIVATE private

    PUBLIC public/Light
    PRIVATE private/Light

    PUBLIC public/Pipeline
    PRIVATE private/Pipeline
    
    PRIVATE private/Descriptor
    PUBLIC public/Descriptor

    PUBLIC public/Buffer
    PRIVATE private/Buffer

    PUBLIC public/Texture
    PRIVATE private/Texture

    PUBLIC public/Material
    PRIVATE private/Material

    PUBLIC public/Mesh
    PRIVATE private/Mesh

    PUBLIC public/Rendering
    PRIVATE private/Rendering
)

target_include_directories(VulkanEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanEngine PUBLIC ${Vulkan_LIBRARIES} RenderApi EngineCore)