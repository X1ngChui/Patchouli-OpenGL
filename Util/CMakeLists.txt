# Source files
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Library definition
add_library(Util STATIC ${sources})

# Include directories
target_include_directories (Util
    PRIVATE
    ${PROJECT_SOURCE_DIR}/lib/xxhash
    ${PROJECT_SOURCE_DIR}/lib/mimalloc/include
    PUBLIC
    ${PROJECT_SOURCE_DIR}/lib/fmt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libruaries
target_link_libraries(Util PRIVATE xxhash mimalloc-static)

# Visual Studio Native Visualizers
if (MSVC)
    target_sources(Util
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/visuallizers/vs/Identifier.natvis
    )
endif()
