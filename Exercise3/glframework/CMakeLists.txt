cmake_minimum_required(VERSION 3.16)

# set the project name
project(GLFramework)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# imgui
add_library(imgui STATIC
    ext/imgui/imgui.cpp
    ext/imgui/imgui_draw.cpp
    ext/imgui/imgui_demo.cpp
    ext/imgui/imgui_tables.cpp
    ext/imgui/imgui_widgets.cpp
    ext/imgui/backends/imgui_impl_glfw.cpp
    ext/imgui/backends/imgui_impl_opengl3.cpp)

target_include_directories(imgui PRIVATE
    ext/glfw/include
    ext/imgui)

add_library(glad STATIC
    ext/glfw/deps/glad_gl.c)

# add the executable
file(GLOB CPP_SOURCES src/*.*)
file(GLOB SHADER_SOURCES shaders/*.*)
add_executable(GLFramework
    ${CPP_SOURCES}
    ${SHADER_SOURCES})

include_directories(GLFramework PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/ext/glm"
    "${PROJECT_SOURCE_DIR}/ext/glfw/deps"
    "${PROJECT_SOURCE_DIR}/ext/imgui"
    "${PROJECT_SOURCE_DIR}/ext/imgui/backends")

add_subdirectory(ext/glfw)
add_subdirectory(ext/glm)

target_link_libraries(GLFramework PUBLIC glm glfw ${GLFW_LIBRARIES} glad imgui)

# shaders
add_custom_target(shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${PROJECT_SOURCE_DIR}/shaders
        ${PROJECT_BINARY_DIR}/shaders)
add_dependencies(GLFramework shaders)

#resources
add_custom_target(resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res
        ${PROJECT_BINARY_DIR}/res)
add_dependencies(GLFramework resources)

#submission
include(${PROJECT_SOURCE_DIR}/CMake_submission.cmake)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFramework)
endif()
