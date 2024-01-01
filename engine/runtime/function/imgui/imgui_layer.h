#pragma once
#include "platform/windows/windows_window.h"
#include "core/layer/layer.h"
#include "core/events/events.h"

#include <ImGuizmo.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <functional>

namespace Leaper
{
    class ImGuiLayer : public Leaper::Layer
    {   
    public:
        ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnEvent(Leaper::Event& e) override;
        void Begin();
        void End();
    private:
    };

} // namespace Leaper
