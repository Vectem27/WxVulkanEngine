project(CompilationTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ajouter votre exécutable
add_executable(${PROJECT_NAME} main.cpp)

set(MINGW_EXTRA_LIBS)
if(MINGW)
	set(MINGW_EXTRA_LIBS "-lversion;-lshlwapi;-luxtheme;-loleacc;-lws2_32;-limm32")
	message("MINGW_EXTRA_LIBS is " ${MINGW_EXTRA_LIBS})
endif(MINGW)

# Spécifier le répertoire des bibliothèques
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${wxLua_LIBRARIES} ${MINGW_EXTRA_LIBS} ${Vulkan_LIBRARIES})