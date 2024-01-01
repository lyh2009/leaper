#include "framebuffer.h"
#include "lppch.h"

#include "render_api.h"
#include "platform/opengl/opengl_framebuffer.h"

Leaper::Ref<Leaper::FrameBuffer> Leaper::FrameBuffer::Create()
{
    switch(Leaper::RenderAPI::GetAPI())
    {
        case Leaper::RenderAPI::API::OpenGL:    return Leaper::CreateRef<OpenGLFrameBuffer>();
    };

    return nullptr;
}
