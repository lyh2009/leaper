#pragma once
#include <GLFW/glfw3.h>
#include "core/base.h"

namespace Leaper
{
    class Context
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetViewPort(float width, float height) = 0;
        static Leaper::Scope<Context> Create(GLFWwindow* window);
    };
} // namespace Leaper
