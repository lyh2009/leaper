cmake_minimum_required(VERSION 3.12.0)

project(Leaper)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory("engine/3rdparty/")
add_subdirectory("engine/runtime/")
add_subdirectory("engine/editor/")



