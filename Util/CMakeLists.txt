# Util Project
cmake_minimum_required(VERSION 3.21)

add_library(util INTERFACE)
target_include_directories(util INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)