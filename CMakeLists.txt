# CMakeList.txt: Root CMake project file where global configuration is performed
# and sub-projects are included.
#
cmake_minimum_required (VERSION 3.21)

project ("Patchouli")

# Set language standard.
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_C_STANDARD 17)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_C_EXTENSIONS OFF)

# Set output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# Set macros
if (WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(PACHE_DEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(PACHE_RELEASE)
endif()

# Set subprojects.
set (SUBPROJECTS "lib/spdlog" "Patchouli" "Spell" )

macro(add_subprojects)
    foreach(subproject ${SUBPROJECTS})
        add_subdirectory(${subproject})
    endforeach()
endmacro()

add_subprojects()