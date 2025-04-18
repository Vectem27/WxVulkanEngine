cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#-------------------------------------------------#
#                   wxWidgets
#-------------------------------------------------#
set(CMAKE_BUILD_TYPE Debug) # Force le mode Debug
set(wxWidgets_USE_DEBUG ON)
set(wxWidgetwxBUILD_SHARED ON)

# Trouver wxWidgets
#set(ENV{WX_CONFIG} "C:/wxWidgets-3.2.6/wx-config")
find_package(wxWidgets 3.2.6 REQUIRED COMPONENTS core base)


message(STATUS "=================================================================")
message(STATUS "wxWidgets_ROOT_DIR=${wxWidgets_ROOT_DIR}")
message(STATUS "wxWidgets_LIB_DIR=${wxWidgets_LIB_DIR}")
message(STATUS "wxWidgets_INCLUDE_DIRS=${wxWidgets_INCLUDE_DIRS}")
message(STATUS "wxWidgets_USE_FILE=${wxWidgets_USE_FILE}")
message(STATUS "=================================================================")


if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
else()
    message(FATAL_ERROR "wxWidgets not found!")
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



set(wxLua_LIBRARIES LuaLib wxLuaLib wxLuaDebugLib wxLuaDebuggerLib wxLuaBindLib)

#-------------------------------------------------#
#                    Vulkan
#-------------------------------------------------#

find_package(Vulkan REQUIRED)


#-------------------------------------------------#
#                     Project
#-------------------------------------------------#










#--Project-Source--#


macro(COPY_wxDependancies copyWxLua)
if(WIN32)
    # wxWidgets dlls copy
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(wxDllsMatchingPattern "*ud_*.dll")
    else()
        set(wxDllsMatchingPattern "*u_*.dll")
    endif()

    file(COPY "${wxWidgets_LIB_DIR}/"
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        FILES_MATCHING PATTERN ${wxDllsMatchingPattern})

            
    # wxLua dlls copy
    if(${copyWxLua})
        file(COPY "${wxLua_ROOT_DIR}/bin/${CMAKE_BUILD_TYPE}/"
            DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            FILES_MATCHING PATTERN "*.dll")
    endif()
endif()
endmacro()

#--Scripts--#
macro(COPY_LuaScripts)
file(COPY "${CMAKE_SOURCE_DIR}/scripts/lua"
    DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scripts/lua"
    FILES_MATCHING PATTERN "*.dll")
endmacro()

set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/shaders)

#--Shaders--#
find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)

macro(COPY_Shaders SHADER_SOURCE_FILES SHADER_OUTPUT_DIR)
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

    set(SHADER_OUTPUT_FILES)
    foreach(SHADER ${SHADER_SOURCE_FILES})
        get_filename_component(FILENAME ${SHADER} NAME)
        set(SPV_FILE ${SHADER_OUTPUT_DIR}/${FILENAME})
        list(APPEND SHADER_OUTPUT_FILES ${SPV_FILE})

        add_custom_command(
            OUTPUT ${SPV_FILE}
            COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER} -o ${SPV_FILE}
            DEPENDS ${SHADER}
            COMMENT "Compiling shader: ${SHADER}"
            VERBATIM
        )
    endforeach()

    add_custom_target(Shaders ALL DEPENDS ${SHADER_OUTPUT_FILES})
endmacro()

set(GLOBAL_RUNTIME_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_RUNTIME_OUTPUT_DIR})

add_subdirectory(modules)
add_subdirectory(src)





