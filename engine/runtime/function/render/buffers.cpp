#include "buffers.h"
#include "function/task/render_task_producer.h"
#include "lppch.h"
#include "platform/opengl/opengl_buffers.h"
#include "render_api.h"

namespace Leaper
{

    Ref<VertexBuffer> VertexBuffer::Create(size_t size)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
        }

        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, size_t size)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t count)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(count);
        }

        return nullptr;
    }
}  // namespace Leaper