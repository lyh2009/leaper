#include <function/render/renderer2d.h>

#include "editor_layer.h"

#include "ImGuizmo.h"
#include "core/base.h"
#include "core/time.h"
#include "function/ecs/components.h"
#include "function/ecs/entity.h"
#include "function/render/texture.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "imgui.h"
#include <core/math/math.h>
#include <function/application/application.h>
#include <function/input/input.h>
#include <function/render/perspective_camera_controller.h>
#include <function/render/render_command.h>
#include <function/render/renderer3d.h>
#include <function/utils/platform_utils.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui_internal.h>

#include <iostream>
#include <string>

extern const std::filesystem::path g_assets_path;

EditorLayer::EditorLayer() : Leaper::Layer("EditorLayer"), m_camera(1024.0f / 648.0f), m_game_camera(1024 / 648) {}

void EditorLayer::OnAttach()
{
    m_hierarchy_window.OnAttach();
    m_project_window.OnAttach();

    // create FrameBuffer
    m_framebuffer = Leaper::FrameBuffer::Create();
    m_framebuffer->CreateTexture(1024, 648, Leaper::TextureFormat::RGB8, Leaper::TextureFormat::RGB, Leaper::Attachments::COLOR_ATTACHMENT0);
    m_framebuffer->CreateTexture(1024, 648, Leaper::TextureFormat::R32I, Leaper::TextureFormat::RED_INTEGER, Leaper::Attachments::COLOR_ATTACHMENT1);
    m_framebuffer->CreateDepthTexture(1024, 648);
    // create Textures
    m_play_icon      = Leaper::Texture::Create("./resource/icons/play.png");
    m_stop_icon      = Leaper::Texture::Create("./resource/icons/stop.png");
    m_translate_icon = Leaper::Texture::Create("./resource/icons/translate.png");
    m_rotate_icon    = Leaper::Texture::Create("./resource/icons/rotate.png");
    m_scale_icon     = Leaper::Texture::Create("./resource/icons/scale.png");

    m_active_scene = Leaper::CreateRef<Leaper::Scene>();
    m_active_scene->OnAttach();

    camera_entity = m_active_scene->CreateEntity("camera");
    camera_entity.AddComponent<Leaper::CameraComponent>(1024.0f / 648.0f);
    camera_entity.AddComponent<Leaper::CubeRendererComponent>();

    m_console.OnAttach();

    m_perspective_camera = Leaper::EditorCamera(30.0f, 1024 / 648, 0.1f, 1000.0f);
}

void EditorLayer::OnUpdate()
{

    Leaper::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    m_framebuffer->Bind();
    m_framebuffer->ClearAttachment(1, -1);

    // resize window
    if (m_viewport_size != *((glm::vec2*)&m_viewport_panel_size))
    {
        m_framebuffer->RescaleFrameBuffer(m_viewport_panel_size.x, m_viewport_panel_size.y);
        m_camera.OnResize(m_viewport_panel_size.x, m_viewport_panel_size.y);
        m_game_camera.OnResize(m_viewport_panel_size.x, m_viewport_panel_size.y);
        m_perspective_camera.SetViewportSize(m_viewport_panel_size.x, m_viewport_panel_size.y);
        camera_entity.GetComponent<Leaper::CameraComponent>().camera.OnResize(m_viewport_panel_size.x, m_viewport_panel_size.y);
        m_viewport_size = { m_viewport_panel_size.x, m_viewport_panel_size.y };
    }

    // scene update
    m_game_camera      = camera_entity.GetComponent<Leaper::CameraComponent>().camera;
    auto& camera_trans = camera_entity.GetComponent<Leaper::TransformComponent>();

    if (m_scene_state == SceneState::Edit)
    {
        m_camera.OnUpdate();
        m_perspective_camera.OnUpdate();

        Leaper::Renderer2D::BeginScene(m_perspective_camera.GetViewProjection());
        Leaper::Renderer3D::BeginScene(m_perspective_camera.GetViewProjection(), m_perspective_camera.GetPosition());
    }
    else if (m_scene_state == SceneState::Play)
    {
        m_game_camera.OnUpdate();
        Leaper::Renderer2D::BeginScene(m_game_camera.GetCamera().GetProjectionMat(), camera_trans.GetTransform());
    }
    m_active_scene->OnUpdate();

    // render rect
    if (m_draw_rect)
    {
        Leaper::Renderer2D::SetLineWidth(1.0f);
        Leaper::TransformComponent transform;
        float ratio        = m_game_camera.GetRatio();
        float zoom_level   = m_game_camera.GetZoomLevel();
        transform.scale    = glm::vec3(ratio * zoom_level * 2, zoom_level * 2, 1.0f);
        transform.position = camera_trans.position;
        transform.rotation = camera_trans.rotation;
        Leaper::Renderer2D::DrawRect(transform, glm::vec4(0.5, 0.5, 0.5, 1), -1);

        auto view = m_active_scene->GetAllEntitiesWith<Leaper::TransformComponent, Leaper::BoxCollider2DComponent>();
        for (auto entity : view)
        {
            auto [tc, bc2d] = view.get<Leaper::TransformComponent, Leaper::BoxCollider2DComponent>(entity);

            Leaper::TransformComponent transform;
            transform.position = tc.position;
            transform.scale    = tc.scale * glm::vec3(bc2d.size * 2.0f, 1.0f);
            transform.rotation = tc.rotation;

            Leaper::Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
        }

        auto cc2d_view = m_active_scene->GetAllEntitiesWith<Leaper::TransformComponent, Leaper::CircleCollider2DComponent>();
        for (auto entity : cc2d_view)
        {
            auto [tc, cc2d] = cc2d_view.get<Leaper::TransformComponent, Leaper::CircleCollider2DComponent>(entity);

            Leaper::TransformComponent transform;
            transform.position = tc.position;
            transform.scale    = tc.scale * cc2d.radiu;
            transform.rotation = tc.rotation;

            Leaper::Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.05f);
        }
    }
    Leaper::Renderer2D::EndScene();
    Leaper::Renderer3D::EndScene();

    // get entity
    if (m_mouse_in_window.x >= 0 && m_mouse_in_window.y >= 0 && m_mouse_in_window.x < m_window_size.x && m_mouse_in_window.x < m_window_size.x)
    {
        int entity_id    = m_framebuffer->ReadPixels(1, (int)m_mouse_in_texture.x, (int)m_mouse_in_texture.y);
        m_hovered_entity = entity_id == -1 ? Leaper::Entity() : Leaper::Entity((entt::entity)entity_id, m_active_scene.get());
    }

    m_framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{
    m_hierarchy_window.SetScene(m_active_scene);

    // DockSpace and MenuBar
    {
        static bool* p_open                       = (bool*)true;
        static bool opt_padding                   = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", p_open, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        // dock space
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 7.0f));

        ImGui::BeginMainMenuBar();
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("Save As..."))
            {
                std::string filepath = Leaper::FileDialogs::SaveFile("Leaper Scene(*.json)\0*.json");
                if (!filepath.empty())
                {
                    Leaper::SceneSerializer serizlizer(m_active_scene);
                    serizlizer.Write(filepath);
                }
            }

            if (ImGui::MenuItem("Open..."))
            {
                std::string filepath = Leaper::FileDialogs::OpenFile("Leaper Scene(*.json)\0*.json");
                if (!filepath.empty())
                {
                    m_hierarchy_window.GetSelected() = {};

                    m_active_scene = Leaper::CreateRef<Leaper::Scene>();
                    m_hierarchy_window.SetScene(m_active_scene);
                    Leaper::SceneSerializer serizlizer(m_active_scene);
                    serizlizer.Read(filepath);
                    m_active_scene->OnAttach();
                    auto view = m_active_scene->Reg().view<Leaper::CameraComponent>();
                    for (auto e : view) { camera_entity = { e, m_active_scene.get() }; }
                }
            }
            ImGui::EndMenu();
        }

        ImGui::ShowDemoWindow();

        ImGui::EndMainMenuBar();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    // scene window
    ImGui::Begin("Scene");
    ImGui::BeginChild("Scene Render");

    m_mouse_pos        = ImGui::GetMousePos();
    m_window_size      = ImGui::GetWindowSize();
    m_window_pos       = ImGui::GetWindowPos();
    m_mouse_in_window  = ImVec2(m_mouse_pos.x - m_window_pos.x, m_mouse_pos.y - m_window_pos.y);
    m_mouse_in_texture = ImVec2(m_mouse_in_window.x, m_window_size.y - m_mouse_in_window.y);

    m_viewport_hovered    = ImGui::IsWindowHovered();
    m_window_size         = ImGui::GetWindowSize();
    m_viewport_pos        = ImGui::GetWindowPos();
    m_viewport_panel_size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight());

    ImGui::Image((void*)m_framebuffer->GetTexture(0), ImVec2(m_viewport_size.x, m_viewport_size.y), ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
        {
            const wchar_t* path              = (const wchar_t*)payload->Data;
            std::filesystem::path filepath   = std::filesystem::path(g_assets_path) / path;
            m_hierarchy_window.GetSelected() = {};

            m_active_scene = Leaper::CreateRef<Leaper::Scene>();
            m_hierarchy_window.SetScene(m_active_scene);

            Leaper::SceneSerializer serizlizer(m_active_scene);
            serizlizer.Read(filepath.string());
            m_active_scene->OnAttach();
        }

        ImGui::EndDragDropTarget();
    }

    // gizmo
    auto select = m_hierarchy_window.GetSelected();
    if (select)
    {
        ImGuizmo::SetDrawlist();

        float window_height = (float)ImGui::GetWindowHeight();
        float window_width  = (float)ImGui::GetWindowWidth();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);

        // gizmo camera

        auto& tc            = select.GetComponent<Leaper::TransformComponent>();
        glm::mat4 transform = tc.GetTransform();
        if (m_scene_state == SceneState::Edit)
        {
            const glm::mat4& camera_projection = m_perspective_camera.GetProjection();
            glm::mat4 camera_view              = m_perspective_camera.GetViewMatrix();

            ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection), (ImGuizmo::OPERATION)m_gizmo, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
                                 nullptr, nullptr, nullptr);
            auto viewManipulateRight = ImGui::GetWindowPos().x + window_width;
            auto viewManipulateTop   = ImGui::GetWindowPos().y;
        }
        else
        {
            Leaper::OrthgraphicCamera& camera  = m_game_camera.GetCamera();
            const glm::mat4& camera_projection = camera.GetProjectionMat();
            glm::mat4 camera_view              = camera.GetViewMat();

            ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection), (ImGuizmo::OPERATION)m_gizmo, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
                                 nullptr, nullptr, nullptr);
        }

        if (Leaper::Input::IsKeyDown(LP_KEY_1)) m_gizmo = ImGuizmo::OPERATION::TRANSLATE;
        if (Leaper::Input::IsKeyDown(LP_KEY_2)) m_gizmo = ImGuizmo::OPERATION::ROTATE;
        if (Leaper::Input::IsKeyDown(LP_KEY_3)) m_gizmo = ImGuizmo::OPERATION::SCALE;

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 position, rotation, scale;
            Leaper::Math::DecomposeTransform(transform, position, rotation, scale);
            glm::vec3 delat_rotation = rotation - tc.rotation;
            tc.position              = position;
            tc.rotation += delat_rotation;
            tc.scale = scale;
        }
    }

    ImGui::EndChild();

    ImGui::End();

    DrawToolBar();
    m_hierarchy_window.OnUpdate();
    m_project_window.OnUpdate();
    m_console.OnUpdate();
}

void EditorLayer::OnEvent(Leaper::Event& e)
{
    m_camera.OnEvent(e);
    m_perspective_camera.OnEvent(e);
    Leaper::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Leaper::MouseButtonPressedEvent>(LP_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
}

bool EditorLayer::OnMouseButtonPressed(Leaper::MouseButtonPressedEvent& e)
{
    if (e.GetMouseButton() == Leaper::Mosue::ButtonLeft)
    {
        if (m_viewport_hovered && !ImGuizmo::IsOver())
        {
            m_hierarchy_window.GetSelected() = m_hovered_entity;
            LP_LOG("entity id: {0}", (uint32_t)m_hovered_entity);
        }
    }
    return false;
}

void EditorLayer::OnSceneEdit()
{
    m_scene_state = SceneState::Edit;
    m_active_scene->OnRuntimeStop();
}

void EditorLayer::OnScenePlay()
{
    m_scene_state = SceneState::Play;
    m_active_scene->OnRuntimeStart();
}

void EditorLayer::DrawToolBar()
{
    // color style
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

    // editor toolbar
    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    // play icon
    Leaper::Ref<Leaper::Texture> button_icon = m_scene_state == SceneState::Edit ? m_play_icon : m_stop_icon;

    bool toolbarEnabled = (bool)m_active_scene;

    ImVec4 tint_color = ImVec4(1, 1, 1, 1);
    if (!toolbarEnabled) tint_color.w = 0.5f;
    float size = ImGui::GetWindowHeight() - 4.0f;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    // play button
    if (ImGui::ImageButton((void*)button_icon->GetTexture(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tint_color))
    {
        if (m_scene_state == SceneState::Edit)
            OnScenePlay();
        else if (m_scene_state == SceneState::Play)
            OnSceneEdit();
    }

    ImGui::SameLine();

    ImVec4 translate_color = m_gizmo == ImGuizmo::OPERATION::TRANSLATE ? ImVec4(1.0f, 1.0f, 1.0f, 0.9f) : ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
    ImVec4 rotate_color    = m_gizmo == ImGuizmo::OPERATION::ROTATE ? ImVec4(1.0f, 1.0f, 1.0f, 0.9f) : ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
    ImVec4 scale_color     = m_gizmo == ImGuizmo::OPERATION::SCALE ? ImVec4(1.0f, 1.0f, 1.0f, 0.9f) : ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
    if (ImGui::ImageButton((void*)m_translate_icon->GetTexture(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), translate_color))
    {
        m_gizmo = ImGuizmo::OPERATION::TRANSLATE;
    }
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)m_rotate_icon->GetTexture(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), rotate_color))
    {
        m_gizmo = ImGuizmo::OPERATION::ROTATE;
    }
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)m_scale_icon->GetTexture(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), scale_color))
    {
        m_gizmo = ImGuizmo::OPERATION::SCALE;
    }
    ImGui::PopStyleColor();
    // CheckBox("DrawRect")
    ImGui::SameLine();
    ImGui::Checkbox("DrawRect", &m_draw_rect);
    ImGui::SameLine();

    // show fps
    ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
    ImGui::PopStyleVar(2);
    ImGui::End();
}
