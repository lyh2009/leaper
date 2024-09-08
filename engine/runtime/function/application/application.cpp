#include "application.h"
#include "function/render/context.h"
#include "function/render/renderer3d.h"
#include "lppch.h"

#include <iostream>
#include <mutex>
#include <thread>

Leaper::Application* Leaper::Application::s_instance = nullptr;
// Leaper::CameraController Leaper::Application::camera_controller(-1.0f, 1.0f, -1.0f, 1.0f);

Leaper::Application::Application(Leaper::RenderAPI::API api, int width, int height, std::string title)
{
    // init LogSystem
    Leaper::Log::Init();

    // Init RenderAPI
    m_render_api = Leaper::RenderAPI::Create(api);
    // Init Window
    m_window = Leaper::Window::Create(width, height, title);

    m_window->SetEventCallback(LP_BIND_EVENT_FN(Application::OnEvent));

    s_instance = this;

    m_render_thread = std::thread(&Application::RenderMain, this);
}

Leaper::Application::~Application()
{
    if (m_render_thread.joinable())
    {
        m_render_thread.join();  // 等待渲染线程结束
    }
}

void Leaper::Application::OnAttach() {}

void Leaper::Application::PushLayer(Leaper::Layer* layer)
{
    m_layer_stack.PushLayer(layer);
}

void Leaper::Application::PushOverlay(Leaper::Layer* overlay)
{
    m_layer_stack.PushOverlay(overlay);
}

void Leaper::Application::Run()
{
    std::lock_guard<std::mutex> mtx_locker(m_mutex);
    m_window->OnUpdate();
}

void Leaper::Application::OnEvent(Leaper::Event& e)
{
    for (Leaper::Layer* layer : m_layer_stack) { layer->OnEvent(e); }
    // for(auto it = m_layer_stack.rbegin(); it != m_layer_stack.rend(); ++it)
    //{
    //     if(e.handle)
    //         break;
    //     (*it)->OnEvent(e);
    // }
}

void Leaper::Application::RenderMain()
{
    m_context = Leaper::Context::Create((GLFWwindow*)m_window->GetNativeWindow());
    m_context->Init();
    m_render_api->Init();
    // Init ImGuiLayer
    m_imgui_layer = new Leaper::ImGuiLayer();
    s_instance    = this;

    // init Renderer
    Renderer2D::Init();
    Leaper::Renderer3D::Init();

    PushOverlay(m_imgui_layer);

    for (Leaper::Layer* layer : m_layer_stack) { layer->OnAttach(); }
    while (!m_window->IsClose())
    {
        Leaper::Time::Begin();

        m_render_api->Clear();

        m_imgui_layer->Begin();
        {
            for (Leaper::Layer* layer : m_layer_stack) layer->OnImGuiRender();
        }
        m_imgui_layer->End();

        for (Leaper::Layer* layer : m_layer_stack) layer->OnUpdate();
        m_context->SwapBuffers();
        Leaper::Time::End();
    }
}