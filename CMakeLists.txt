cmake_minimum_required(VERSION 3.10)
project(WxVulkanEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Spécifier le répertoire racine de wxWidgets
set(wxWidgets_ROOT_DIR C:/wxWidgets-3.2.6)

set(wxWidgets_LIB_DIR ./thirdparty/wxWidgets-3.2.6/lib)
set(wxWidgets_INCLUDE_DIRS ./thirdparty/wxWidgets-3.2.6/include)

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

# Ajouter votre exécutable
add_executable(${PROJECT_NAME} WIN32 src/main.cpp)

# Inclure les répertoires d'en-tête de wxWidgets
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})

# Spécifier le répertoire des bibliothèques
target_link_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_LIB_DIR})

# Linker les bibliothèques wxWidgets
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Copier le répertoire des DLL dans le répertoire de sortie
        file(COPY "${wxWidgets_LIB_DIR}"
            DESTINATION ${PROJECT_BINARY_DIR}
            FILES_MATCHING PATTERN "*32ud_*.dll")
    else()
        # Copier le répertoire des DLL dans le répertoire de sortie
        file(COPY "${wxWidgets_LIB_DIR}"
        DESTINATION ${PROJECT_BINARY_DIR}
        FILES_MATCHING PATTERN "*32u_*.dll")
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Copier le répertoire des DLL dans le répertoire de sortie
    file(COPY "${wxWidgets_LIB_DIR}"
        DESTINATION ${PROJECT_BINARY_DIR}
        FILES_MATCHING PATTERN "*32ud_*.a")
    else()
    # Copier le répertoire des DLL dans le répertoire de sortie
    file(COPY "${wxWidgets_LIB_DIR}"
    DESTINATION ${PROJECT_BINARY_DIR}
    FILES_MATCHING PATTERN "*32u_*.a")
    endif()
endif()
