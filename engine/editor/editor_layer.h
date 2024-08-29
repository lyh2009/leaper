#pragma once
#include "function/render/orthographic_camera.h"
#include "panels/console.h"
#include "panels/hierarchy.h"
#include "panels/project.h"

#include "core/events/events.h"
#include "core/layer/layer.h"
#include "function/ecs/scene_serializer.h"
#include "function/render/camera_controller.h"
#include "function/render/game_camera.h"
#include "function/render/perspective_camera_controller.h"

#include <ImGuizmo.h>
#include <imgui.h>

class EditorLayer : public Leaper::Layer
{
public:
    EditorLayer();
    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;

    virtual void OnEvent(Leaper::Event& e) override;

    inline Hierarchy& GetHierarchyWindow()
    {
        return m_hierarchy_window;
    }
    inline Project& GetProjectWindow()
    {
        return m_project_window;
    }

private:
    bool OnMouseButtonPressed(Leaper::MouseButtonPressedEvent& e);

private:
    void OnSceneEdit();
    void OnScenePlay();
    void DrawToolBar();

private:
    Hierarchy m_hierarchy_window;
    Project m_project_window;
    Console m_console;

    bool m_draw_rect = false;
    bool m_viewport_hovered;
    int m_gizmo = ImGuizmo::OPERATION::TRANSLATE;
    ImVec2 m_viewport_panel_size;
    glm::vec2 m_viewport_size;
    ImVec2 m_viewport_pos;

    ImVec2 m_mouse_pos;
    ImVec2 m_window_size;
    ImVec2 m_window_pos;
    ImVec2 m_mouse_in_window;
    ImVec2 m_mouse_in_texture;

    Leaper::Entity camera_entity;
    Leaper::Entity m_hovered_entity;

    Leaper::CameraController m_camera;
    Leaper::GameCamera m_game_camera;
    Leaper::EditorCamera m_perspective_camera;

    Leaper::Ref<Leaper::Scene> m_active_scene;
    Leaper::Ref<Leaper::FrameBuffer> m_framebuffer;
    Leaper::Ref<Leaper::Texture> m_play_icon;
    Leaper::Ref<Leaper::Texture> m_stop_icon;
    Leaper::Ref<Leaper::Texture> m_translate_icon;
    Leaper::Ref<Leaper::Texture> m_rotate_icon;
    Leaper::Ref<Leaper::Texture> m_scale_icon;

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