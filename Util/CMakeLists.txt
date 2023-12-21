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


if (MSVC_IDE)
    option(VS_ADD_NATIVE_VISUALIZERS "Configure project to use Visual Studio native visualizers" TRUE)
else()
    set(VS_ADD_NATIVE_VISUALIZERS FALSE CACHE INTERNAL "Native visualizers are Visual Studio extension" FORCE)
endif()

# add natvis file to the library so it will automatically be loaded into Visual Studio
if(VS_ADD_NATIVE_VISUALIZERS)
    target_sources(GSL INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/visuallizers/vs/Identifier.natvis
    )
endif()
