set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(3RDPARTY ../3rdparty)

file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c" "*.hpp")
file(GLOB_RECURSE ALL_3RDPARTY_SOURCE"${3RDPARTY}/glm/*.hpp" "${3RDPARTY}/imgui/*.cpp" "${3RDPARTY}/imgui_zmo/*.cpp" "${3RDPARTY}/glad/*.c")

set(INCLUDE_DIR
 "./"
 "${3RDPARTY}/glad/include/"
 "${3RDPARTY}/glfw/include/"
 "${3RDPARTY}/box2d/include/"
 "${3RDPARTY}/glm/"
 "${3RDPARTY}/imgui/"
 "${3RDPARTY}/imgui_zmo/"
 "${3RDPARTY}/spdlog/"
 "${3RDPARTY}/lua-5.4.4/"
 "${3RDPARTY}/sol2/include/"
 )

 
 include_directories(${INCLUDE_DIR})
 add_library(leaper_runtime STATIC ${ALL_SOURCE} ${ALL_3RDPARTY_SOURCE})
 
 target_link_libraries(leaper_runtime PUBLIC
 glfw
 opengl32
 box2d
 spdlog
 lua_static
 )