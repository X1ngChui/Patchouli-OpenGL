# Collect source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create a library from the collected sources
add_library(Patchouli STATIC ${sources})

# Link libraries
target_link_libraries(Patchouli PRIVATE glfw glad imgui stbimage Util mimalloc-static)

# Set include directories 
target_include_directories (Patchouli
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/Util/include
        ${PROJECT_SOURCE_DIR}/lib/fmt/include
        ${PROJECT_SOURCE_DIR}/lib/spdlog/include
        ${PROJECT_SOURCE_DIR}/lib/mimalloc/include
        ${PROJECT_SOURCE_DIR}/lib/glm
        ${PROJECT_SOURCE_DIR}/lib/imgui
    PRIVATE
        ${PROJECT_SOURCE_DIR}/lib/glfw/include
        ${PROJECT_SOURCE_DIR}/lib/glad/include
        ${PROJECT_SOURCE_DIR}/lib/stb_image/include        
)

# Set precompiled header
target_precompile_headers(Patchouli PRIVATE "include/Patchoulipch.h")

# Macro definition
define_config_macros(Patchouli)
add_compile_definitions(GLFW_INCLUDE_NONE)
