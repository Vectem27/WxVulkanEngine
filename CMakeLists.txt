cmake_minimum_required(VERSION 3.10)
project(WxVulkanEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




#-------------------------------------------------#
#                   wxWidgets
#-------------------------------------------------#
set(CMAKE_BUILD_TYPE Debug) # Force le mode Debug
set(wxWidgets_USE_DEBUG TRUE)

# Trouver wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
else()
    message(FATAL_ERROR "wxWidgets not found!")
endif()

message(STATUS "=================================================================")
message(STATUS "wxWidgets_ROOT_DIR=${wxWidgets_ROOT_DIR}")
message(STATUS "wxWidgets_LIB_DIR=${wxWidgets_LIB_DIR}")
message(STATUS "wxWidgets_INCLUDE_DIRS=${wxWidgets_INCLUDE_DIRS}")
message(STATUS "wxWidgets_USE_FILE=${wxWidgets_USE_FILE}")
message(STATUS "=================================================================")




if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Copier le répertoire des DLL dans le répertoire de sortie
        file(COPY "${wxWidgets_LIB_DIR}/"
            DESTINATION ${PROJECT_BINARY_DIR}
            FILES_MATCHING PATTERN "*32u_*.dll")
    else()
        # Copier le répertoire des DLL dans le répertoire de sortie
        file(COPY "${wxWidgets_LIB_DIR}/"
        DESTINATION ${PROJECT_BINARY_DIR}
        FILES_MATCHING PATTERN "*32ud_*.dll")
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Copier le répertoire des DLL dans le répertoire de sortie
    file(COPY "${wxWidgets_LIB_DIR}/"
        DESTINATION ${PROJECT_BINARY_DIR}
        FILES_MATCHING PATTERN "*32u_*.a")
    else()
    # Copier le répertoire des DLL dans le répertoire de sortie
    file(COPY "${wxWidgets_LIB_DIR}/"
    DESTINATION ${PROJECT_BINARY_DIR}
    FILES_MATCHING PATTERN "*32ud_*.a")
    endif()
endif()


#-------------------------------------------------#
#                     wxLua
#-------------------------------------------------#

set(wxLua_ROOT_DIR ${CMAKE_SOURCE_DIR}/thirdparty/wxlua/wxLua)
set(wxLua_DIR ${wxLua_ROOT_DIR}/build)

#set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
#add_subdirectory(${wxLua_ROOT_DIR})
find_package(wxLua REQUIRED)


include_directories("${wxLua_ROOT_DIR}/"
                    "${wxLua_ROOT_DIR}/modules/"
                    "${wxLua_ROOT_DIR}/modules/lua-5.1/src")

message(STATUS "=================================================================")
message(STATUS "wxLua_ROOT_DIR=${wxLua_ROOT_DIR}")
message(STATUS "wxLua_DIR=${wxLua_DIR}")
message(STATUS "modules=${wxLua_ROOT_DIR}/modules/")
message(STATUS "wxLua_LUA_INCLUDE_DIR=${wxLua_ROOT_DIR}/modules/lua-5.1/src")
message(STATUS "wxLuaBind_ALL_LIBS=${wxLuaBind_ALL_LIBS}")
message(STATUS "=================================================================")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Copier le répertoire des DLL dans le répertoire de sortie
    file(COPY "${wxLua_ROOT_DIR}/bin/Release/"
        DESTINATION ${PROJECT_BINARY_DIR}
        FILES_MATCHING PATTERN "*.dll")
else()
    # Copier le répertoire des DLL dans le répertoire de sortie
    file(COPY "${wxLua_ROOT_DIR}/bin/Debug/"
    DESTINATION ${PROJECT_BINARY_DIR}
    FILES_MATCHING PATTERN "*.dll")
endif()

#-------------------------------------------------#
#                    Vulkan
#-------------------------------------------------#


find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS})
link_directories(${Vulkan_LIBRARY_DIRS} ${wxWidgets_LIBRARY_DIRS})


#-------------------------------------------------#
#                     Project
#-------------------------------------------------#


# Ajouter votre exécutable
add_executable(${PROJECT_NAME} WIN32 src/main.cpp)

file(COPY "${CMAKE_SOURCE_DIR}/scripts"
     DESTINATION "${CMAKE_BINARY_DIR}/scripts")



set(MINGW_EXTRA_LIBS)
if(MINGW)
	set(MINGW_EXTRA_LIBS "-lversion;-lshlwapi;-luxtheme;-loleacc;-lws2_32;-limm32")
	message("MINGW_EXTRA_LIBS is " ${MINGW_EXTRA_LIBS})
endif(MINGW)

# Spécifier le répertoire des bibliothèques
target_link_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_LIB_DIR})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} LuaLib wxLuaLib wxLuaDebugLib wxLuaDebuggerLib wxLuaBindLib ${MINGW_EXTRA_LIBS} ${Vulkan_LIBRARIES})