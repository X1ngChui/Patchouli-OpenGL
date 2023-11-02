# Patchouli Project
cmake_minimum_required(VERSION 3.21)

# Collect source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create a library from the collected sources
add_library(${subproject} STATIC ${sources})

# Set include directories
target_include_directories (${subproject} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/spdlog/include
)

# Set the output directory for the library
set_target_properties(${subproject} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
