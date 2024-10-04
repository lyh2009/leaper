#include "shader_library.h"

namespace Leaper
{
    Ref<Shader> ShaderLibrary::s_diffault_shader  = nullptr;
    Ref<Shader> ShaderLibrary::s_depth_map_shader = nullptr;
    Ref<Shader> ShaderLibrary::s_skybox_shader    = nullptr;
    void ShaderLibrary::Init()
    {
        s_diffault_shader  = Shader::CreateNative("./resource/shaders/3d/model.glsl");
        s_depth_map_shader = Shader::CreateNative("./resource/shaders/depth_map.glsl");
        s_skybox_shader    = Shader::CreateNative("./resource/shaders/skybox.glsl");
    }

}  // namespace Leaper