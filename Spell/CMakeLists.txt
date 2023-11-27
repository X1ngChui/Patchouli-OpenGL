# Spell Project
cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
add_executable(Spell ${sources} "include/Spell.h")

target_include_directories(Spell PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Spell with Patchouli library
target_link_libraries(Spell PRIVATE Patchouli)

# Specify the location of Patchouli header files
target_include_directories(Spell PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Patchouli/include
)

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(Spell PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()
