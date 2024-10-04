#pragma once
#include <function/render/shader.h>

namespace Leaper
{
    class ShaderLibrary
    {
    public:
        static void Init();
        static Ref<Shader> GetDeffaultShader()
        {
            return s_diffault_shader;
        }
        static Ref<Shader> GetDepthMapShader()
        {
            return s_depth_map_shader;
        }
        static Ref<Shader> GetSkyBoxShader()
        {
            return s_skybox_shader;
        }

    private:
        static Ref<Shader> s_diffault_shader;
        static Ref<Shader> s_depth_map_shader;
        static Ref<Shader> s_skybox_shader;
    };
}  // namespace Leaper