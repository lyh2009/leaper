#include "opengl_context.h"
#include "lppch.h"

#include "core/base.h"

#include <iostream>
#include <sstream>

OpenGLContext::OpenGLContext(GLFWwindow *window)
    : m_window(window)
{
    LP_ASSERT(window, "'window' is null!");
}

OpenGLContext::~OpenGLContext()
{
    glfwDestroyWindow(m_window);
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_window);
    bool status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    LP_ASSERT(status, "Failed to initialize Glad!");

}

void OpenGLContext::SetViewPort(float width, float height)
{
    glViewport(0, 0, width, height);
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}
