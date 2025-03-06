add_library(VulkanEngine public/VulkanEngine.h)
target_include_directories(ModuleA 
    PUBLIC public
    PRIVATE private
)