#pragma once

#include "core/base.h"
#include "core/events/events.h"
#include "function/application/window.h"
#include "function/render/context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <functional>
#include <string>

class WindowsWindow : public Leaper::Window
{
public:
    WindowsWindow(const std::string& title, uint32_t width, uint32_t height);

    virtual std::string GetTitle() const override
    {
        return m_data.title;
    };
    virtual uint32_t GetWidth() const override
    {
        return m_data.width;
    };
    virtual uint32_t GetHeight() const override
    {
        return m_data.height;
    };

    virtual void SetEventCallback(const EventCallbackFunc& event) override
    {
        m_data.event_callback = event;
    }

    virtual void OnUpdate() override;
    virtual void OnClose() override;

    virtual void* GetNativeWindow() const override
    {
        return m_window;
    };

    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override
    {
        return m_data.VSync;
    };

    virtual bool IsClose() const override
    {
        return glfwWindowShouldClose(m_window);
    }

private:
    virtual void Init() override;

private:
    GLFWwindow* m_window;

    struct WindowData
    {
        std::string title;
        uint32_t width, height;
        bool VSync;
        EventCallbackFunc event_callback;
    };
    WindowData m_data;
};