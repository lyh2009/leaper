cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


project(Leaper)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Debug)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Release)
else ()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
endif ()



add_subdirectory("./engine/3rdparty/")
add_subdirectory("./engine/runtime/")
add_subdirectory("./engine/editor/")



