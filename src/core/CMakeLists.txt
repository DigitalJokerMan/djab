cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE CORE_SOURCES "*.cpp")
add_library(djab_core ${CORE_SOURCES})
target_include_directories(djab_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
