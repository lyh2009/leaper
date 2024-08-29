#include "buffers.h"
#include "lppch.h"
#include "platform/opengl/opengl_buffers.h"
#include "render_api.h"


Leaper::Ref<Leaper::VertexBuffer> Leaper::VertexBuffer::Create(size_t size)
{
    switch (Leaper::RenderAPI::GetAPI())
    {
    case Leaper::RenderAPI::API::OpenGL: return Leaper::CreateRef<OpenGLVertexBuffer>(size);
    }

    return nullptr;
}

Leaper::Ref<Leaper::VertexBuffer> Leaper::VertexBuffer::Create(const void* vertices, size_t size)
{
    switch (Leaper::RenderAPI::GetAPI())
    {
    case Leaper::RenderAPI::API::OpenGL: return Leaper::CreateRef<OpenGLVertexBuffer>(vertices, size);
    }
    return nullptr;
}

Leaper::Ref<Leaper::IndexBuffer> Leaper::IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
    switch (Leaper::RenderAPI::GetAPI())
    {
    case Leaper::RenderAPI::API::OpenGL: return Leaper::CreateRef<OpenGLIndexBuffer>(indices, count);
    }

    return nullptr;
}

Leaper::Ref<Leaper::IndexBuffer> Leaper::IndexBuffer::Create(uint32_t count)
{
    switch (Leaper::RenderAPI::GetAPI())
    {
    case Leaper::RenderAPI::API::OpenGL: return Leaper::CreateRef<OpenGLIndexBuffer>(count);
    }

    return nullptr;
}