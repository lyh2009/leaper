#include "function/render/render_api.h"
#include "lppch.h"
#include "platform/opengl/opengl_uniform_buffer.h"
#include "uniform_buffer.h"

namespace Leaper
{

    Ref<UniformBuffer> UniformBuffer::Create(size_t size, uint32_t binding)
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::None: return nullptr;
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(size, binding);
        }

        return nullptr;
    }

}  // namespace Leaper