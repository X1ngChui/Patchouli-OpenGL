# glad Project
cmake_minimum_required(VERSION 3.21)

# Collect source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create a library from the collected sources
add_library(glad STATIC ${sources})

# Set include directories
target_include_directories (glad 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)