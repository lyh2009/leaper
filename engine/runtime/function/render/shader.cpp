#include "shader.h"
#include "lppch.h"

#include "render_api.h"
#include "platform/opengl/opengl_shader.h"

Leaper::Ref<Leaper::Shader> Leaper::Shader::Create(std::string vertex_shader_path, std::string fragment_shader_path)
{
    switch (Leaper::RenderAPI::GetAPI())
    {  
        case Leaper::RenderAPI::API::OpenGL:
            return Leaper::CreateRef<OpenGLShader>(vertex_shader_path, fragment_shader_path);
    }
    return nullptr;
}