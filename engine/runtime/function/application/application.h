#pragma once

#include "platform/windows/windows_window.h"
#include "function/application/window.h"
#include "function/render/renderer2d.h"
#include "function/ecs/components.hpp"
#include "function/ecs/entity.h"
#include "function/imgui/imgui_layer.h"
#include "function/render/camera_controller.h"
#include "core/time.h"
#include "core/log.h"
#include "core/layer/layer.h"
#include "core/layer/layer_stack.h"

#include <string.h>

namespace Leaper
{
    class Application
    {
    public:
        Application(Leaper::RenderAPI::API api, int width, int height, std::string title);
        ~Application();

        static Application &Get() { return *s_instance; }

        inline Leaper::Ref<Leaper::Window> GetWindow() { return m_window; }
        inline Leaper::Ref<Leaper::RenderAPI> GetRenderAPI() { return m_render_api; }
        inline Leaper::ImGuiLayer *GetImGuiLayer() const { return m_imgui_layer; }
        void OnAttach();

        void PushLayer(Leaper::Layer *layer);
        void PushOverlay(Leaper::Layer *overlay);

        void OnEvent(Leaper::Event &e);

        void Run();

    private:
        static Application *s_instance;

        Leaper::ImGuiLayer *m_imgui_layer = nullptr;
        Leaper::Ref<Leaper::Window> m_window;

        Leaper::Ref<Leaper::RenderAPI> m_render_api;
        Leaper::LayerStack m_layer_stack;
    };

} // namespace Leaper
