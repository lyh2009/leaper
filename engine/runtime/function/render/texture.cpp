#include "lppch.h"
#include "texture.h"

#include "platform/opengl/opengl_texture.h"
#include "render_api.h"
namespace Leaper
{

    Ref<Texture> Texture::Create(std::string path, bool is_flip)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::None: return nullptr;
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLTexture>(path, is_flip);
        }
        return nullptr;
    }

    Ref<Texture> Texture::Create(uint32_t width, uint32_t height)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::None: return nullptr;
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLTexture>(width, height);
        }
        return nullptr;
    }

    Ref<Texture> Texture::CreateCubeMap(std::vector<std::string> faces)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::None: return nullptr;
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLTexture>(faces);
        }
        return nullptr;
    }
}  // namespace Leaper