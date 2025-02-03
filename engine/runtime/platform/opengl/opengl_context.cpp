#include "core/log.h"
#include "lppch.h"
#include "opengl_context.h"

#include "core/base.h"

#include <iostream>
#include <sstream>
namespace Leaper
{
    OpenGLContext::OpenGLContext(GLFWwindow* window) : m_window(window)
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

        auto vendor   = glGetString(GL_VENDOR);
        auto version  = glGetString(GL_VERSION);
        auto renderer = glGetString(GL_RENDERER);
        LP_CORE_INFO("Verndor: {0}", vendor);
        LP_CORE_INFO("Renderr: {0}", renderer);
        LP_CORE_INFO("Version: {0}", version);
    }

    void OpenGLContext::SetViewPort(float width, float height)
    {
        glViewport(0, 0, width, height);
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

}  // namespace Leaper