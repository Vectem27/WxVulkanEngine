project(WxVulkanEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_RUNTIME_OUTPUT_DIR}/${PROJECT_NAME})
COPY_wxDependancies(ON)
COPY_LuaScripts()


set(SHADER_SOURCE_FILES  
	${SHADER_SOURCE_DIR}/shader.vert
	${SHADER_SOURCE_DIR}/shader.frag
	${SHADER_SOURCE_DIR}/shadowmap.vert
	${SHADER_SOURCE_DIR}/fullScreen.vert
	${SHADER_SOURCE_DIR}/lighting.frag
	${SHADER_SOURCE_DIR}/postprocess.frag
)

set(SHADER_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

if(COMPILE_SHADERS)
	COPY_Shaders("${SHADER_SOURCE_FILES}" ${SHADER_OUTPUT_DIR})
endif()

add_executable(${PROJECT_NAME} main.cpp App/App.cpp App/VulkanFrame.cpp)

set(MINGW_EXTRA_LIBS)
if(MINGW)
	set(MINGW_EXTRA_LIBS "-lversion;-lshlwapi;-luxtheme;-loleacc;-lws2_32;-limm32")
	message("MINGW_EXTRA_LIBS is " ${MINGW_EXTRA_LIBS})
endif(MINGW)


target_include_directories(${PROJECT_NAME} PUBLIC Engine ${Vulkan_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC 
	${wxWidgets_LIBRARIES} 
	LuaLib wxLuaLib wxLuaDebugLib wxLuaDebuggerLib wxLuaBindLib 
	${MINGW_EXTRA_LIBS} 
	${Vulkan_LIBRARIES} 
	Engine
	Rendering
)









