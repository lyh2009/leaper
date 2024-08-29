#include "opengl_input.h"
#include "lppch.h"

#include "function/application/application.h"

#include <GLFW/glfw3.h>

bool OpenGLInput::IsKeyDownImpl(int key_code)
{
    GLFWwindow* window = (GLFWwindow*)Leaper::Application::Get().GetWindow()->GetNativeWindow();
    int state = glfwGetKey(window, key_code);
    
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool OpenGLInput::IsKeyReleaseImpl(int key_code)
{
    GLFWwindow* window = (GLFWwindow*)Leaper::Application::Get().GetWindow()->GetNativeWindow();
    int state = glfwGetKey(window, key_code);
    
    return state == GLFW_RELEASE;
}

bool OpenGLInput::IsMouseDownImpl(int key_code)
{
    GLFWwindow* window = (GLFWwindow*)Leaper::Application::Get().GetWindow()->GetNativeWindow();
    int state = glfwGetMouseButton(window, key_code);
    return state == GLFW_PRESS || state == GLFW_RELEASE;
}