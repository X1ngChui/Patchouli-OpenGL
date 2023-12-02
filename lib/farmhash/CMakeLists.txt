# farmhash Project
cmake_minimum_required(VERSION 3.21)

# Collect source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create a library from the collected sources
add_library(farmhash STATIC ${sources} "include/farmhash.h" "src/farmhash.cc")

# Set Macros
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(FARMHASH_NO_BUILTIN_EXPECT)
else()
    add_compile_definitions(FARMHASH_OPTIONAL_BUILTIN_EXPECT)
endif()

# Set include directories
target_include_directories (farmhash PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)