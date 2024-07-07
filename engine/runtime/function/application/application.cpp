#include "application.h"
#include "lppch.h"

#include <iostream>

Leaper::Application *Leaper::Application::s_instance = nullptr;
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

    // Init ImGuiLayer
    m_imgui_layer = new Leaper::ImGuiLayer();
    s_instance = this;

    // init Renderer
    Renderer2D::Init(m_render_api);

    PushOverlay(m_imgui_layer);
}

Leaper::Application::~Application()
{
}

void Leaper::Application::OnAttach()
{
}

void Leaper::Application::PushLayer(Leaper::Layer *layer)
{
    m_layer_stack.PushLayer(layer);
    layer->OnAttach();
}

void Leaper::Application::PushOverlay(Leaper::Layer *overlay)
{
    m_layer_stack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Leaper::Application::Run()
{
    Leaper::Time::Begin();

    m_render_api->Clear();

    for (Leaper::Layer *layer : m_layer_stack)
        layer->OnUpdate();

    m_imgui_layer->Begin();
    {
        for (Leaper::Layer *layer : m_layer_stack)
            layer->OnImGuiRender();
    }
    m_imgui_layer->End();

    m_window->OnUpdate();

    Leaper::Time::End();
}

void Leaper::Application::OnEvent(Leaper::Event &e)
{
    for (Leaper::Layer *layer : m_layer_stack)
    {
        layer->OnEvent(e);
    }
    // for(auto it = m_layer_stack.rbegin(); it != m_layer_stack.rend(); ++it)
    //{
    //     if(e.handle)
    //         break;
    //     (*it)->OnEvent(e);
    // }
}