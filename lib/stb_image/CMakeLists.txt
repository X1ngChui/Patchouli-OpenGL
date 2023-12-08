# stb_image Project
cmake_minimum_required(VERSION 3.21)

# Collect source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create a library from the collected sources
add_library(stbimage STATIC ${sources})

# Set include directories
target_include_directories (stbimage PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)