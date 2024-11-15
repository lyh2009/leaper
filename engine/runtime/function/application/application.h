#pragma once

#include "core/events/application_event.h"
#include "core/layer/layer.h"
#include "core/layer/layer_stack.h"
#include "core/log.h"
#include "core/time.h"
#include "function/application/window.h"
#include "function/imgui/imgui_layer.h"
#include "function/render/context.h"
#include "function/render/render_api.h"
#include "function/task/render_task_queue.h"
#include "function/task/render_task_types.h"
#include <mutex>

#include <string.h>
#include <thread>

namespace Leaper
{
    class Application
    {
    public:
        Application(RenderAPI::API api, int width, int height, std::string title);
        ~Application();

        static Application& Get()
        {
            return *s_instance;
        }

        inline Ref<Window> GetWindow()
        {
            return m_window;
        }
        inline Ref<RenderAPI> GetRenderAPI()
        {
            return m_render_api;
        }
        inline ImGuiLayer* GetImGuiLayer() const
        {
            return m_imgui_layer;
        }

        void OnAttach();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void OnEvent(Event& e);
        bool OnWindowResize(WindowResizeEvent& e);

        void Run();

    private:
        void EndFrame(RenderTaskBase* task);

        static Application* s_instance;

        ImGuiLayer* m_imgui_layer = nullptr;
        Ref<Window> m_window;

        Ref<RenderAPI> m_render_api;
        Scope<Context> m_context;
        LayerStack m_layer_stack;

        std::thread m_render_thread;
        std::mutex m_mutex;
        bool is_attach   = false;
        bool m_minimized = false;

        // RenderTaskBase* m_render_task;
    };

}  // namespace Leaper
