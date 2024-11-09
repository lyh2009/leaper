#pragma once
#include "core/base.h"
#include "function/ecs/components.h"
#include "function/ecs/entity.h"
#include "function/ecs/scene.h"
#include "function/render/framebuffer.h"
#include "function/render/texture.h"

#include <ImGuizmo.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include <filesystem>

namespace Leaper
{
    class Hierarchy
    {
    public:
        Hierarchy() = default;
        Hierarchy(const Leaper::Ref<Leaper::Scene>& scene);

        void OnAttach(std::string_view assets_path);
        void OnImGuiRender();

        inline Leaper::Entity& GetSelected()
        {
            return m_selected;
        }
        void SetScene(const Leaper::Ref<Leaper::Scene>& scene)
        {
            m_active_scene = scene;
        }

        inline Leaper::Ref<Leaper::FrameBuffer>& GetFrameBuffer()
        {
            return m_framebuffer;
        }

    private:
        void DrawEntityNode(Leaper::Entity entity);
        void DrawComponents(Leaper::Entity entity);
        static void DrawVector(const std::string& label, glm::vec3& values, float speed);

        const glm::vec2 GetMousePosInImguiWindow();

        template <typename T, typename DrawFunc> void DrawComponent(const std::string& name, Leaper::Entity entity, DrawFunc func);

        Leaper::Entity m_selected;
        Leaper::Ref<Leaper::Scene> m_active_scene;
        Leaper::Ref<Leaper::FrameBuffer> m_framebuffer;
        Leaper::Ref<Leaper::Texture> m_entity_icon;
        Leaper::Ref<Leaper::Texture> m_setting_icon;

        std::filesystem::path m_assets_path;

        int m_gizmo = ImGuizmo::OPERATION::TRANSLATE;

        static bool is_instance;
    };
}  // namespace Leaper