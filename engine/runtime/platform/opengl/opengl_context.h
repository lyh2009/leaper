#pragma once
#include "function/render/context.h"

#include <glad/glad.h>

class GLFWwindow;
class OpenGLContext : public Leaper::Context
{
public:
    OpenGLContext(GLFWwindow *window);
    ~OpenGLContext();
    virtual void Init() override;
    virtual void SetViewPort(float width, float height) override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow *m_window;
};
