# Patchouli Project
cmake_minimum_required(VERSION 3.21)

# Collect source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create a library from the collected sources
add_library(${subproject} STATIC ${sources} "include/Core/Window.h" "include/Patchoulipch.h" "src/Patchoulipch.cpp" "include/Platform/Windows/WindowsWindow.h" "src/Platform/Windows/WindowsWindow.cpp")

# Set include directories
target_include_directories (${subproject} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/spdlog/include
    ${PROJECT_SOURCE_DIR}/lib/glfw/include
)

# Link libruaries
target_link_libraries(${subproject} PRIVATE glfw)

# Set precompiled header
target_precompile_headers (${subproject} PRIVATE "include/Patchoulipch.h" "src/Patchoulipch.cpp")

# Set the output directory for the library
set_target_properties(${subproject} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
