set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(3RDPARTY ../3rdparty)

file(GLOB_RECURSE ALL_SOURCE
 "*.cpp"
 "*.c"
 "${3RDPARTY}/imgui/backends/*.cpp" 
 "${3RDPARTY}/imgui/*.cpp" 
 "${3RDPARTY}/imgui_zmo/*.cpp"
 "${3RDPARTY}/meshoptimizer/*.cpp"
 )
set(INCLUDE_DIR
 "./"
 "${3RDPARTY}/glad/include/"
 "${3RDPARTY}/glfw/include/"
 "${3RDPARTY}/box2d/include/"
 "${3RDPARTY}/stb/include/"
 "${3RDPARTY}/glm/"
 "${3RDPARTY}/imgui/"
 "${3RDPARTY}/imgui_zmo/"
 "${3RDPARTY}/spdlog/"
 "${3RDPARTY}/lua-5.4.4/"
 "${3RDPARTY}/sol2/include/"
 "${3RDPARTY}/FMOD/api/core/inc/"
 "${3RDPARTY}/FMOD/api/fsbank/inc/"
 "${3RDPARTY}/FMOD/api/studio/inc/"
 "${3RDPARTY}/assimp/include"
 "${3RDPARTY}/meshoptimizer"
 "${3RDPARTY}/IconFontCppHeaders")

 include_directories(${INCLUDE_DIR})
add_library(leaper_runtime STATIC ${ALL_SOURCE})

 target_link_libraries(leaper_runtime 
 PUBLIC glfw
 Glad
 opengl32
 glm
 box2d
 spdlog
 lua_static
 assimp
 PUBLIC FMOD::Core FMOD::Fsbank FMOD::Studio
 )