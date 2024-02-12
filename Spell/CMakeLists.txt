file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
add_executable(Spell ${sources})           

# Custom target for resources
add_custom_target(UpdateResources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Spell>/assets
    COMMENT "Updating resources..."
)

# Make Spell target depend on UpdateResources
add_dependencies(Spell UpdateResources)

# Macro definitions
define_config_macros(Spell)

# Link Spell with Patchouli library
target_link_libraries(Spell PRIVATE Patchouli)

target_include_directories(Spell PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
