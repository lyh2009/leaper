#include "render_api.h"
#include "lppch.h"

#include "platform/opengl/opengl_render_api.h"
#include "core/base.h"

Leaper::RenderAPI::API Leaper::RenderAPI::m_static_api = Leaper::RenderAPI::API::OpenGL;

Leaper::Ref<Leaper::RenderAPI> Leaper::RenderAPI::Create(API api)
{
    switch(api)
    {
    case Leaper::RenderAPI::API::None: return nullptr;
    case Leaper::RenderAPI::API::OpenGL:
        m_static_api = Leaper::RenderAPI::API::OpenGL;
        return Leaper::CreateRef<OpenGLRenderAPI>();
    }
    
    return nullptr;
}
