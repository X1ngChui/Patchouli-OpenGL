# Spell Project

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
add_executable(Spell ${sources})

target_include_directories(Spell PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Spell with Patchouli library
target_link_libraries(${subproject} PRIVATE Patchouli)

# Specify the location of Patchouli header files
target_include_directories(${subproject} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../Patchouli/include
)