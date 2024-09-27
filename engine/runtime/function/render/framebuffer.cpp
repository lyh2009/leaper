#include "core/base.h"
#include "framebuffer.h"

#include "platform/opengl/opengl_framebuffer.h"
#include "render_api.h"

namespace Leaper
{

    Ref<FrameBuffer> FrameBuffer::Create()
    {
        switch (RenderAPI::GetAPI())
        {
        case RenderAPI::API::None: return nullptr;
        case RenderAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>();
        };

        return nullptr;
    }

}  // namespace Leaper