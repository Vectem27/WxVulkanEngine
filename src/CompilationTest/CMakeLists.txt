project(CompilationTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_RUNTIME_OUTPUT_DIR}/${PROJECT_NAME})
#COPY_wxDependancies(ON)
#COPY_LuaScripts()

add_executable(${PROJECT_NAME} main.cpp)


#set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME})

# Spécifier le répertoire des bibliothèques
target_include_directories(${PROJECT_NAME} PRIVATE 
	#${Vulkan_INCLUDE_DIRS} 
	#${wxWidgets_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PRIVATE 
	#${wxWidgets_LIBRARIES} 
	#LuaLib wxLuaLib wxLuaDebugLib wxLuaDebuggerLib wxLuaBindLib 
	#${MINGW_EXTRA_LIBS} 
	#${Vulkan_LIBRARIES} 
	EngineCore
)