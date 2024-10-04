set(RUNTIME leaper_runtime)

set(3RDPARTY ../3rdparty)
set(VulkanDir ${3RDPARTY}/VulkanSDK)
if(WIN32)
   if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      file(GLOB_RECURSE VulkanSDK_Lib
       "${VulkanDir}/Win32/Lib/dxcompiler.lib"
       "${VulkanDir}/Win32/Lib/shadercd.lib"
       "${VulkanDir}/Win32/Lib/shaderc_utild.lib"
       "${VulkanDir}/Win32/Lib/spirv-cross-cored.lib"
       "${VulkanDir}/Win32/Lib/spirv-cross-glsld.lib"
       "${VulkanDir}/Win32/Lib/glslangd.lib"
       "${VulkanDir}/Win32/Lib/SPIRVd.lib"
       "${VulkanDir}/Win32/Lib/HLSLd.lib"
       "${VulkanDir}/Win32/Lib/MachineIndependentd.lib"
       "${VulkanDir}/Win32/Lib/GenericCodeGend.lib"
       "${VulkanDir}/Win32/Lib/OSDependentd.lib"
       "${VulkanDir}/Win32/Lib/OGLCompilerd.lib")
   endif()
   if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
      file(GLOB_RECURSE VulkanSDK_Lib
      "${VulkanDir}/Win32/Lib/dxcompiler.lib"
       "${VulkanDir}/Win32/Lib/shaderc.lib"
       "${VulkanDir}/Win32/Lib/shaderc_util.lib"    
       "${VulkanDir}/Win32/Lib/spirv-cross-core.lib"
       "${VulkanDir}/Win32/Lib/spirv-cross-glsl.lib")
   endif()

endif()

file(GLOB_RECURSE ALL_SOURCE
 "*.cpp"
 "*.c"
 "${3RDPARTY}/imgui/backends/*.cpp" 
 "${3RDPARTY}/imgui/*.cpp" 
 "${3RDPARTY}/ImGuizmo/*.cpp"
 "${3RDPARTY}/meshoptimizer/*.cpp"
 )
set(INCLUDE_DIR
 "./"
 "${3RDPARTY}/glad/include/"
 "${3RDPARTY}/glfw/include/"
 "${3RDPARTY}/box2d/include/"
 "${3RDPARTY}/stb/include/"
 "${3RDPARTY}/magic_enum/include/"
 "${3RDPARTY}/SPSCQueue/include/"
 "${3RDPARTY}/glm/"
 "${3RDPARTY}/imgui/"
 "${3RDPARTY}/ImGuizmo/"
 "${3RDPARTY}/spdlog/"
 "${3RDPARTY}/lua-5.4.4/"
 "${3RDPARTY}/sol2/include/"
 "${3RDPARTY}/FMOD/api/core/inc/"
 "${3RDPARTY}/FMOD/api/fsbank/inc/"
 "${3RDPARTY}/FMOD/api/studio/inc/"
 "${3RDPARTY}/assimp/include"
 "${3RDPARTY}/meshoptimizer"
 "${3RDPARTY}/IconFontCppHeaders"
 "${VulkanDir}/Include/")
 
 
 include_directories(${INCLUDE_DIR})
 add_library(${RUNTIME} STATIC ${ALL_SOURCE})
 target_precompile_headers(${RUNTIME} PUBLIC "./lppch.h")

 target_link_libraries(${RUNTIME}
    PUBLIC PUBLIC glfw
    PUBLIC Glad
    PUBLIC opengl32
    PUBLIC glm
    PUBLIC box2d
    PUBLIC spdlog
    PUBLIC lua_static
    ${VulkanSDK_Lib}
    PUBLIC assimp
    PUBLIC FMOD::Core FMOD::Fsbank FMOD::Studio
 )