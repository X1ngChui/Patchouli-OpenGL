cmake_minimum_required(VERSION 3.20)

# Project name
project("Patchouli")

# Set language standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Set output directories for executable, library, and symbol files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/dist/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/dist/${CMAKE_BUILD_TYPE})
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/dist/${CMAKE_BUILD_TYPE})
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/dist/${CMAKE_BUILD_TYPE})

# Compiler-specific settings
if(MSVC)
    # MSVC compiler settings
    # Release mode settings (no pdb, optimization enabled)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /fp:fast")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox /fp:fast")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /INCREMENTAL:NO /OPT:REF /OPT:ICF")

    # Debug mode settings (with pdb, no optimization)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
    
    # Testing mode settings (with pdb, optimization enabled)
    set(CMAKE_CXX_FLAGS_TESTING "${CMAKE_CXX_FLAGS_TESTING} /Ox /Zi /fp:fast")
    set(CMAKE_C_FLAGS_TESTING "${CMAKE_C_FLAGS_TESTING} /Ox /Zi /fp:fast")
    set(CMAKE_EXE_LINKER_FLAGS_TESTING "${CMAKE_EXE_LINKER_FLAGS_TESTING} /INCREMENTAL:NO /OPT:REF /OPT:ICF /DEBUG")

    # Hot reload settings
    if(POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,Testing>:EditAndContinue>,$<$<CONFIG:Debug,Testing>:ProgramDatabase>>")
    endif()
else()
    # GNU or Clang compiler settings
    # Release mode settings (no pdb, optimization enabled)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -ffast-math")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -ffast-math")

    # Debug mode settings (with gdb, no optimization)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

    # Testing mode settings (with gdb, optimization enabled)
    set(CMAKE_CXX_FLAGS_TESTING "${CMAKE_CXX_FLAGS_TESTING} -Ofast -g -ffast-math")
    set(CMAKE_C_FLAGS_TESTING "${CMAKE_C_FLAGS_TESTING} -Ofast -g -ffast-math")

    # Enable additional compiler warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Include cmake functions
include("cmakes/ConfigMacros.cmake")
include("cmakes/Library.cmake")

# Add libraries
add_library_directory(fmt)
add_library_directory(spdlog)
add_library_directory(xxhash/cmake)
add_library_directory(glm)
add_library_directory(stb_image)
add_library_directory(glfw)
add_library_directory(glad)
add_library_directory(imgui)
add_library_directory(mimalloc)

# Add subdirectories
add_subdirectory(Util)
add_subdirectory(Patchouli)
add_subdirectory(Spell)
