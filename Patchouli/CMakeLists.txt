# Patchouli Project
cmake_minimum_required(VERSION 3.21)

# Collect source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create a library from the collected sources
add_library(Patchouli STATIC ${sources} "include/Core/Input.h" "include/Platform/Windows/WindowsInput.h" "src/Platform/Windows/WindowsInput.cpp"  "include/Renderer/GraphicsContext.h" "include/Platform/OpenGL/OpenGLContext.h" "src/Platform/OpenGL/OpenGLContext.cpp" "include/Renderer/Shader.h" "src/Renderer/Shader.cpp" "include/Renderer/VertexBuffer.h" "include/Renderer/IndexBuffer.h" "src/Renderer/VertexBuffer.cpp" "src/Renderer/IndexBuffer.cpp" "include/Platform/OpenGL/OpenGLVertexBuffer.h" "src/Platform/OpenGL/OpenGLVertexBuffer.cpp" "include/Renderer/Renderer.h" "src/Renderer/Renderer.cpp" "include/Platform/OpenGL/OpenGLIndexBuffer.h" "src/Platform/OpenGL/OpenGLIndexBuffer.cpp" "include/Renderer/VertexArray.h" "src/Renderer/VertexArray.cpp" "include/Platform/OpenGL/OpenGLVertexArray.h" "src/Platform/OpenGL/OpenGLVertexArray.cpp" "include/Renderer/RendererAPI.h" "include/Renderer/RenderCommand.h" "src/Renderer/RenderCommand.cpp" "include/Platform/OpenGL/OpenGLRendererAPI.h" "src/Platform/OpenGL/OpenGLRendererAPI.cpp" "include/Renderer/OrthographicCamera.h" "src/Renderer/OrthographicCamera.cpp" "include/Patchoulipch.h" "include/Core/Timestep.h" "src/Platform/OpenGL/OpenGLShader.cpp" "include/Platform/OpenGL/OpenGLShader.h" "include/Core/Base.h" "include/Renderer/Texture.h" "src/Renderer/Texture.cpp" "include/Platform/OpenGL/OpenGLTexture.h")

# Link libruaries
target_link_libraries(Patchouli PRIVATE glfw glad imgui stbimage Util)

# Set include directories 
target_include_directories (Patchouli PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Util/include
    ${PROJECT_SOURCE_DIR}/lib/spdlog/include
    ${PROJECT_SOURCE_DIR}/lib/spdlog/include
    ${PROJECT_SOURCE_DIR}/lib/glfw/include
    ${PROJECT_SOURCE_DIR}/lib/glad/include
    ${PROJECT_SOURCE_DIR}/lib/stb_image/include
    ${PROJECT_SOURCE_DIR}/lib/imgui
    ${PROJECT_SOURCE_DIR}/lib/glm

)

# Set precompiled header
target_precompile_headers(Patchouli PRIVATE "include/Patchoulipch.h" "src/Patchoulipch.cpp")

add_compile_definitions(GLFW_INCLUDE_NONE)
