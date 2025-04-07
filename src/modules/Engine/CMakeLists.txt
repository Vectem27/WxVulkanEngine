add_library(Engine STATIC
    private/Material.cpp
    private/Mesh.cpp
)

target_include_directories(Engine 
    PUBLIC public
    PRIVATE private
)

target_include_directories(Engine PUBLIC World VulkanEngine)
target_link_libraries(Engine PUBLIC World VulkanEngine)