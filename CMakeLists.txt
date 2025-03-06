cmake_minimum_required(VERSION 3.10)

#-------------------------------------------------#
#                   wxWidgets
#-------------------------------------------------#
set(CMAKE_BUILD_TYPE Debug) # Force le mode Debug
set(wxWidgets_USE_DEBUG ON)

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
            DESTINATION ${PROJECT_BINARY_DIR}/bin/Release
            FILES_MATCHING PATTERN "*32u_*.dll")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Copier le répertoire des DLL dans le répertoire de sortie
        file(COPY "${wxWidgets_LIB_DIR}/"
        DESTINATION ${PROJECT_BINARY_DIR}/bin/Release
        FILES_MATCHING PATTERN "*32u*.dll")
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Copier le répertoire des DLL dans le répertoire de sortie
        file(COPY "${wxWidgets_LIB_DIR}/"
            DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug
            FILES_MATCHING PATTERN "*32u_*.a")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Copier le répertoire des DLL dans le répertoire de sortie
        file(COPY "${wxWidgets_LIB_DIR}/"
            DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug
            FILES_MATCHING PATTERN "*32u_*.a")
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
message(STATUS "=================================================================")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Copier le répertoire des DLL dans le répertoire de sortie
    file(COPY "${wxLua_ROOT_DIR}/bin/Release/"
        DESTINATION ${PROJECT_BINARY_DIR}/bin/Release
        FILES_MATCHING PATTERN "*.dll")
    file(COPY "${wxLua_ROOT_DIR}/bin-shared/Release/"
        DESTINATION ${PROJECT_BINARY_DIR}/bin/Release
        FILES_MATCHING PATTERN "*.dll")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Copier le répertoire des DLL dans le répertoire de sortie
    file(COPY "${wxLua_ROOT_DIR}/bin/Debug/"
        DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug
        FILES_MATCHING PATTERN "*.dll")
    file(COPY "${wxLua_ROOT_DIR}/bin-shared/Debug/"
        DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug
        FILES_MATCHING PATTERN "*.dll")
endif()

set(wxLua_LIBRARIES LuaLib wxLuaLib wxLuaDebugLib wxLuaDebuggerLib wxLuaBindLib)

#-------------------------------------------------#
#                    Vulkan
#-------------------------------------------------#

find_package(Vulkan REQUIRED)

#-------------------------------------------------#
#                     Project
#-------------------------------------------------#

#--Project-Source--#
add_subdirectory(src)

#--Scripts--#
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(COPY "${CMAKE_SOURCE_DIR}/scripts"
        DESTINATION "${CMAKE_BINARY_DIR}/bin/Release/scripts")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(COPY "${CMAKE_SOURCE_DIR}/scripts"
        DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug/scripts")
endif()



