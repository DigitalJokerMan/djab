cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE WRITER_SOURCES "*.cpp")
add_executable(djab_writer ${WRITER_SOURCES})
target_compile_features(djab_writer PUBLIC cxx_std_17) # Use C++17
target_link_libraries(djab_writer djab_core)
target_include_directories(djab_writer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
