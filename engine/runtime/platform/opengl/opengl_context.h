#pragma once
#include "function/render/context.h"

#include <glad/glad.h>

class GLFWwindow;

namespace Leaper
{

    class OpenGLContext : public Context
    {
    public:
        OpenGLContext(GLFWwindow* window);
        ~OpenGLContext();
        virtual void Init() override;
        virtual void SetViewPort(float width, float height) override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_window;
    };
}  // namespace Leaper
