#include "windows_window.h"
#include "lppch.h"

#include "core/events/mouse_event.h"
#include "core/events/key_event.h"
#include "core/events/application_event.h"

WindowsWindow::WindowsWindow(const std::string &title, uint32_t width, uint32_t height)
{
    m_data.width = width;
    m_data.height = height;
    m_data.title = title;

    Init();
}

void WindowsWindow::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);

    LP_ASSERT(m_window, "Failed to initialize GLFW");

    m_context = Leaper::Context::Create(m_window);
    m_context->Init(); // init glad

    glfwSetWindowUserPointer(m_window, &m_data);

    // call back
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Leaper::MouseScrolledEvent event((float)xoffset, (float)yoffset);
        data.event_callback(event); 
    });
    
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window,int button, int action, int mods)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				Leaper::MouseButtonPressedEvent event(button);
				data.event_callback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				Leaper::MouseButtonReleasedEvent event(button);
				data.event_callback(event);
				break;
			}
		}
    });

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Leaper::WindowResizeEvent event((uint32_t)width, (uint32_t)height);
        data.event_callback(event); 
    });
}

void WindowsWindow::OnUpdate()
{
    glfwPollEvents();
    SetVSync(true);
    m_context->SwapBuffers();
}

void WindowsWindow::OnClose()
{
    delete m_window;
}

void WindowsWindow::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
}