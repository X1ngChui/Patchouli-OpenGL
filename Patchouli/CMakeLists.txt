# Patchouli Project
cmake_minimum_required(VERSION 3.21)

# Collect source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create a library from the collected sources
add_library(Patchouli STATIC ${sources} "include/Core/Window.h" "include/Patchoulipch.h" "src/Patchoulipch.cpp" "include/Platform/Windows/WindowsWindow.h" "src/Platform/Windows/WindowsWindow.cpp" "include/Core/Layer.h" "src/Core/Layer.cpp" "include/Core/EventQueue.h" "src/Core/EventQueue.cpp")

# Set include directories
target_include_directories (Patchouli PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/spdlog/include
    ${PROJECT_SOURCE_DIR}/lib/glfw/include
    ${PROJECT_SOURCE_DIR}/lib/glad/include
)

# Link libruaries
target_link_libraries(Patchouli PRIVATE glfw glad)

# Set precompiled header
target_precompile_headers(Patchouli PRIVATE "include/Patchoulipch.h" "src/Patchoulipch.cpp")

add_compile_definitions(GLFW_INCLUDE_NONE)
