#include "application.h"
#include "core/events/application_event.h"
#include "function/application/window.h"
#include "function/render/context.h"
#include "function/render/renderer2d.h"
#include "function/render/renderer3d.h"
#include "function/task/render_task_queue.h"
#include "function/task/render_task_types.h"
#include "lppch.h"
#include <thread>

#define LP_MUTEX() std::lock_guard<std::mutex> mtx_locker(m_mutex)

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

    m_render_thread = std::thread(&Application::RenderMain, this);
    m_render_thread.detach();
    s_instance = this;
}

Leaper::Application::~Application()
{
    if (m_render_thread.joinable())
        m_render_thread.join();
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
        if (!m_minimized)
        {

            m_render_api->Clear();
            for (Leaper::Layer* layer : m_layer_stack) layer->OnUpdate();
            m_imgui_layer->Begin();
            for (Leaper::Layer* layer : m_layer_stack) layer->OnImGuiRender();
            m_imgui_layer->End();
            m_context->SwapBuffers();
        }

        m_window->OnUpdate();
        Leaper::Time::End();
    }
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

bool Leaper::Application::OnWindowResize(Leaper::WindowResizeEvent& e)
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

void Leaper::Application::EndFrame(RenderTaskBase* task)
{
    auto end = dynamic_cast<RenderTask_EndFrame*>(task);
    m_context->SwapBuffers();
    task->return_result_set = true;
}

void Leaper::Application::RenderMain()
{
    /*
    while (!m_window->IsClose())
    {

        while (true)
        {
            if (RenderTaskQueue::Empty())
            {  // 渲染线程一直等待主线程发出任务。没有了任务Sleep 1微秒。
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                continue;
            }

            RenderTaskBase* render_task           = RenderTaskQueue::Front();
            RenderTaskCommand render_task_command = render_task->commands;
            bool need_return_result               = render_task->need_return_result;

            switch (render_task_command)
            {
            case NONE: break;
            case MODEL_CREATE: RenderTaskCunsumer::Model_Create(render_task);
            case END_FRAME: EndFrame(render_task); break;
            }
            RenderTaskQueue::Pop();
            // LP_CORE_LOG(magic_enum::enum_name(render_task_command));

            if (need_return_result == false)
            {
                delete render_task;
            }

            if (render_task_command == END_FRAME)
                break;
        }
    }
    */
}