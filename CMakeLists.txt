cmake_minimum_required (VERSION 3.21)
project ("Patchouli")

# Build options.
option(FORCE_COMPILE "Enable forced build when the compiler is not supported." OFF)

# Set language standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR FORCE_COMPILE))
    message(FATAL_ERROR "Unsupported compiler, please use the PATCHOULI_FORCE_COMPILE option.")
endif()

# Set output directories.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/debug/dist)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/debug/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/debug/lib)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/release/dist)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/release/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/release/lib)
endif()

# Set macros.
# Set build mode macros.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(PACHE_DEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(PACHE_RELEASE)
endif() 

# Set subprojects.
set(SUBDIRS
    "lib/fmt"
    "lib/spdlog"
    "lib/xxhash/cmake"
    "lib/glm"
    "lib/stb_image"
    "lib/glfw"
    "lib/glad"
    "lib/imgui" 
    "Util"
    "Patchouli"
    "Spell"
)

foreach(SUBDIR ${SUBDIRS})
    add_subdirectory(${SUBDIR})
endforeach()
