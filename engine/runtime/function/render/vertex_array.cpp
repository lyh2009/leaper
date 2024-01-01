#include "vertex_array.h"
#include "lppch.h"

#include "render_api.h"
#include "platform/opengl/opengl_vertex_array.h"

Leaper::Ref<Leaper::VertexArray> Leaper::VertexArray::Create()
{
    switch (Leaper::RenderAPI::GetAPI())
    {
        case Leaper::RenderAPI::API::OpenGL:    return Leaper::CreateRef<OpenGLVertexArray>();
    }

    return nullptr;
}