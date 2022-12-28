cmake_minimum_required(VERSION 3.3)
project(sniper)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES src/*.cpp src/glad/glad.c)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "-framework OpenGL -Wall -Wno-unused-command-line-argument -g")
else()
    find_package(OpenGL REQUIRED)
endif()

add_executable(sniper ${SOURCES})
target_include_directories(sniper PRIVATE src)
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(sniper ${OPENGL_gl_LIBRARY} glfw)
else()
    target_link_libraries(sniper ${OPENGL_gl_LIBRARY} glfw OpenGL)
endif()