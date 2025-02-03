#include "core/log.h"
#include "lppch.h"
#include "vertex_array.h"

#include "platform/opengl/opengl_vertex_array.h"
#include "render_api.h"

namespace Leaper
{
    Ref<Leaper::VertexArray> VertexArray::Create()
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::None: return nullptr;
        case RenderAPI::API::OpenGL: return Leaper::CreateRef<OpenGLVertexArray>();
        }

        LP_CRITICAL("Unknow render API!!!");
        return nullptr;
    }
}  // namespace Leaper