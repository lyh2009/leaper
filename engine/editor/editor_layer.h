#pragma once
#include "function/render/orthographic_camera.h"
#include "panels/console.h"
#include "panels/content_broswer.h"
#include "panels/hierarchy.h"

#include "core/events/events.h"
#include "core/layer/layer.h"
#include "function/ecs/scene_serializer.h"
#include "function/render/camera_controller.h"
#include "function/render/game_camera.h"
#include "function/render/perspective_camera_controller.h"

#include <ImGuizmo.h>
#include <imgui.h>
#include <string_view>
namespace Leaper
{

    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;

        void InitPanels(std::string_view assets_path);

        inline Hierarchy& GetHierarchyWindow()
        {
            return m_hierarchy_window;
        }
        inline ContentBroswer& GetContentBroswerWindow()
        {
            return m_content_broswer_window;
        }

    private:
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

    private:
        void OnSceneEdit();
        void OnScenePlay();
        void DrawToolBar();

    private:
        Hierarchy m_hierarchy_window;
        ContentBroswer m_content_broswer_window;
        Console m_console;

        std::filesystem::path m_assets_path;

        bool m_draw_rect = false;
        bool m_viewport_hovered;
        int m_gizmo = ImGuizmo::OPERATION::TRANSLATE;
        ImVec2 m_viewport_panel_size;
        glm::vec2 m_viewport_size = { 0.0f, 0.0f };
        ImVec2 m_viewport_pos;

        ImVec2 m_mouse_pos;
        ImVec2 m_window_size;
        ImVec2 m_window_pos;
        ImVec2 m_mouse_in_window;
        ImVec2 m_mouse_in_texture;

        Entity camera_entity;
        Entity m_hovered_entity;

        CameraController m_camera;
        GameCamera m_ortho_camera;
        EditorCamera m_perspective_camera;

        Ref<Scene> m_active_scene;
        Ref<FrameBuffer> m_framebuffer;
        Ref<Texture> m_play_icon;
        Ref<Texture> m_stop_icon;
        Ref<Texture> m_translate_icon;
        Ref<Texture> m_rotate_icon;
        Ref<Texture> m_scale_icon;

        glm::vec2 m_viewport_bounds[2];
        enum CameraTypes
        {
            Ortho             = 0,
            ContentBroswerion = 1
        };
        CameraTypes m_editor_camera_type = ContentBroswerion;

        bool is_read_pixel = false;

        struct MousePickingData
        {
            ImVec2 mouse_pos_in_texture;
            ImVec2 mouse_pos_in_window;
            ImVec2 window_size;
            ImVec2 window_pos;
            ImVec2 mouse_pos;
        };
        MousePickingData m_mouse_picking_data;

        enum class SceneState
        {
            Edit = 0,
            Play = 1
        };
        SceneState m_scene_state = SceneState::Edit;
    };
}  // namespace Leaper