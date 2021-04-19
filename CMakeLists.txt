cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

project(oglTessellatedFlag)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(source)
add_subdirectory(external)
