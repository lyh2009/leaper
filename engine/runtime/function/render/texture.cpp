#include "lppch.h"
#include "texture.h"


#include "platform/opengl/opengl_texture.h"
#include "render_api.h"

Leaper::Ref<Leaper::Texture> Leaper::Texture::Create(std::string path, bool is_flip)
{
    switch (Leaper::RenderAPI::GetAPI())
    {
    case Leaper::RenderAPI::API::OpenGL: return Leaper::CreateRef<OpenGLTexture>(path, is_flip);
    }
    return nullptr;
}

Leaper::Ref<Leaper::Texture> Leaper::Texture::Create(uint32_t width, uint32_t height)
{
    switch (Leaper::RenderAPI::GetAPI())
    {
    case Leaper::RenderAPI::API::OpenGL: return Leaper::CreateRef<OpenGLTexture>(width, height);
    }
    return nullptr;
}
