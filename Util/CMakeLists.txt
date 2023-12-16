# Util Project
cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(Util STATIC ${sources})

target_include_directories (Util PUBLIC
    ${PROJECT_SOURCE_DIR}/lib/xxhash
    ${PROJECT_SOURCE_DIR}/lib/fmt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libruaries
target_link_libraries(Util PUBLIC xxhash)