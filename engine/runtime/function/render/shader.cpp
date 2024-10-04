#include "lppch.h"
#include "shader.h"

#include "platform/opengl/opengl_native_shader.h"
#include "platform/opengl/opengl_shader.h"
#include "render_api.h"

namespace Leaper
{

    Ref<Shader> Shader::Create(const std::string& shader_path)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLShader>(shader_path);
        }
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, std::string vertex_shader_path, std::string fragment_shader_path)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLShader>(name, vertex_shader_path, fragment_shader_path);
        }
        return nullptr;
    }

    Ref<Leaper::Shader> Shader::CreateNative(const std::string& shader_path)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLNativeShader>(shader_path);
        }
        return nullptr;
    }
}  // namespace Leaper