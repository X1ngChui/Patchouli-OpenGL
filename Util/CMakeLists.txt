# Util Project
cmake_minimum_required(VERSION 3.21)

# Source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Library definition
add_library(Util STATIC ${sources} "include/ResourcePtr.h")

# Include directories
target_include_directories (Util
    PRIVATE
    ${PROJECT_SOURCE_DIR}/lib/xxhash
    PUBLIC
    ${PROJECT_SOURCE_DIR}/lib/fmt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libruaries
target_link_libraries(Util PUBLIC xxhash)


# Visual Studio Native Visualizers
if (MSVC)
    option(VS_ADD_NATIVE_VISUALIZERS "Configure project to use Visual Studio native visualizers" TRUE)
    if(VS_ADD_NATIVE_VISUALIZERS)
        target_sources(Util
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/visuallizers/vs/Identifier.natvis
        )
    endif()
endif()
