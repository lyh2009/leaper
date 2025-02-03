#include "application.h"
#include "core/events/application_event.h"
#include "core/leap_profiler.hpp"
#include "function/application/window.h"
#include "function/render/context.h"
#include "function/render/renderer2d.h"
#include "function/render/renderer3d.h"
#include "resource/shader_library.h"
#include <thread>

namespace Leaper
{

    Application* Application::s_instance = nullptr;

    Application::Application(RenderAPI::API api, int width, int height, std::string title)
    {
        // init LogSystem
        Log::Init();

        // Init RenderAPI
        m_render_api = RenderAPI::Create(api);
        // Init Window
        m_window = Window::Create(width, height, title);

        m_window->SetEventCallback(LP_BIND_EVENT_FN(Application::OnEvent));
        s_instance = this;
    }

    Application::~Application()
    {
        if (m_render_thread.joinable())
            m_render_thread.join();
    }

    void Application::OnAttach()
    {
        m_context = Context::Create((GLFWwindow*)m_window->GetNativeWindow());
        m_context->Init();
        m_render_api->Init();

        m_imgui_layer = new ImGuiLayer();
        s_instance    = this;

        ShaderLibrary::Init();
        Renderer3D::Init();
        Renderer2D::Init();

        PushOverlay(m_imgui_layer);

        for (Layer* layer : m_layer_stack) { layer->OnAttach(); }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layer_stack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_layer_stack.PushOverlay(overlay);
    }

    void Application::Run()
    {

        Time::Begin();

        if (!m_minimized)
        {

            m_render_api->Clear();
            for (Layer* layer : m_layer_stack) layer->OnUpdate();
            m_imgui_layer->Begin();
            for (Layer* layer : m_layer_stack) layer->OnImGuiRender();
            m_imgui_layer->End();
            m_context->SwapBuffers();
        }

        m_window->OnUpdate();
        LeapProfiler::GetInstance().Rest();
        Time::End();
    }

    void Application::OnEvent(Event& e)
    {
        for (Layer* layer : m_layer_stack) { layer->OnEvent(e); }
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        float width  = e.GetWidth();
        float height = e.GetHeight();
        if (width == 0 || height == 0)
        {
            m_minimized = true;
            return false;
        }

        m_minimized = false;
        return false;
    }
}  // namespace Leaper