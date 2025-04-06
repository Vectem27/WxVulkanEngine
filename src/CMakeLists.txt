set(MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules)

add_subdirectory(${MODULES_DIR})

if(BUILD_App)
    add_subdirectory(App)
endif()

if(BUILD_CompilationTest)
    add_subdirectory(CompilationTest)
endif()

if(BUILD_Test)
    add_subdirectory(test)
endif()