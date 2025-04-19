file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(VulkanRendering STATIC 
    ${SRC_FILES}
    ${HEADER_FILES}
)

target_include_directories(VulkanRendering 
    PUBLIC
        public
    PRIVATE
        private
)

target_link_libraries(VulkanRendering 
    PUBLIC
        EngineCore
        RHI
        VulkanAPI
)