set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(3RDPARTY ../3rdparty)
set(LIBRARY ../../../build/engine/3rdparty)

file(COPY ../resource DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ../../assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(EDITOR_INCLUDE_DIR
 "${3RDPARTY}/glm/"
 "${3RDPARTY}/imgui/"
 "${3RDPARTY}/ImGuizmo/"
 "${3RDPARTY}/IconFontCppHeaders/"
 "${3RDPARTY}/SPSCQueue/include/"
 "${3RDPARTY}/magic_enum/include/"
 "${3RDPARTY}/lua-5.4.4/"
 "${3RDPARTY}/sol2/include/"
 "../runtime/"
 )

 file(GLOB_RECURSE SRC_DIR
  "*.cpp"
  "planes/*.cpp"
  "${3RDPARTY}/imgui/*.cpp" 
  "${3RDPARTY}/ImGuizmo/*.cpp" 
  )

 include_directories(${EDITOR_INCLUDE_DIR})

 add_executable(leaper ${SRC_DIR})

 target_link_libraries(leaper
 leaper_runtime
 )