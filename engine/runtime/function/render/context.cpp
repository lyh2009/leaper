#include "lppch.h"

#include "render_api.h"
#include "context.h"
#include "platform/opengl/opengl_context.h"

Leaper::Scope<Leaper::Context> Leaper::Context::Create(GLFWwindow* window)
{
    switch (Leaper::RenderAPI::GetAPI())
    {
        case Leaper::RenderAPI::API::OpenGL:    return Leaper::CreateScope<OpenGLContext>(window);
    }

    return nullptr;
}