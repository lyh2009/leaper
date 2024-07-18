#include <function/render/renderer2d.h>

#include "editor_layer.h"

#include "core/base.h"
#include "core/time.h"
#include "function/ecs/components.h"
#include "function/ecs/entity.h"
#include <core/math/math.h>
#include <function/application/application.h>
#include <function/input/input.h>
#include <function/utils/platform_utils.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui_internal.h>

#include <iostream>

extern const std::filesystem::path g_assets_path;

EditorLayer::EditorLayer() : Leaper::Layer("EditorLayer"), m_camera(1024.0f / 648.0f), m_game_camera(1024 / 648) {}

void EditorLayer::OnAttach()
{
    m_hierarchy_window.OnAttach();
    m_project_window.OnAttach();

    // create FrameBuffer
    m_framebuffer = Leaper::FrameBuffer::Create();
    m_framebuffer->CreateTexture(1024, 648, Leaper::TextureFormat::RGB8, Leaper::TextureFormat::RGB,
                                 Leaper::Attachments::COLOR_ATTACHMENT0);
    m_framebuffer->CreateTexture(1024, 648, Leaper::TextureFormat::R32I, Leaper::TextureFormat::RED_INTEGER,
                                 Leaper::Attachments::COLOR_ATTACHMENT1);

    // create Textures
    m_play_icon = Leaper::Texture::Create("resource\\icons\\play.png");
    m_stop_icon = Leaper::Texture::Create("resource\\icons\\stop.png");

    m_active_scene = Leaper::CreateRef<Leaper::Scene>();
    m_active_scene->OnAttach();

    camera_entity = m_active_scene->CreateEntity("camera");
    camera_entity.AddComponent<Leaper::CameraComponent>(1024.0f / 648.0f);
    camera_entity.AddComponent<Leaper::SoundComponent>("assets\\ikun2.mp3");
}

void EditorLayer::OnUpdate()
{

    m_framebuffer->Bind();
    m_framebuffer->ClearAttachment(1, -1);

    // resize window
    if (m_viewport_size != *((glm::vec2*)&m_viewport_panel_size))
    {
        m_framebuffer->RescaleFrameBuffer(m_viewport_panel_size.x, m_viewport_panel_size.y);
        m_camera.OnResize(m_viewport_panel_size.x, m_viewport_panel_size.y);
        m_game_camera.OnResize(m_viewport_panel_size.x, m_viewport_panel_size.y);
        m_viewport_size = { m_viewport_panel_size.x, m_viewport_panel_size.y };
        camera_entity.GetComponent<Leaper::CameraComponent>().camera.OnResize(m_viewport_panel_size.x,
                                                                              m_viewport_panel_size.y);
    }

    // scene update
    m_game_camera      = camera_entity.GetComponent<Leaper::CameraComponent>().camera;
    auto& camera_trans = camera_entity.GetComponent<Leaper::TransformComponent>();
    if (m_scene_state == SceneState::Edit)
    {
        m_camera.OnUpdate();
        Leaper::Renderer2D::BeginScene(m_camera.GetCamera());
    }
    else if (m_scene_state == SceneState::Play)
    {
        m_game_camera.OnUpdate();
        Leaper::Renderer2D::BeginScene(m_game_camera.GetCamera(), camera_trans.GetTransform());
    }
    m_active_scene->OnUpdate();

    // render rect
    if (m_draw_rect)
    {
        Leaper::Renderer2D::SetLineWidth(2.0f);
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

            Leaper::Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1), -1);
        }
    }
    Leaper::Renderer2D::EndScene();

    // get entity
    if (m_mouse_in_window.x >= 0 && m_mouse_in_window.y >= 0 && m_mouse_in_window.x < m_window_size.x
        && m_mouse_in_window.x < m_window_size.x)
    {
        int entity_id = m_framebuffer->ReadPixels(1, (int)m_mouse_in_texture.x, (int)m_mouse_in_texture.y);
        m_hovered_entity =
            entity_id == -1 ? Leaper::Entity() : Leaper::Entity((entt::entity)entity_id, m_active_scene.get());
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
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                        | ImGuiWindowFlags_NoMove;
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
                    for (auto e : view)
                    {
                        camera_entity = { e, m_active_scene.get() };
                    }
                }
            }
            ImGui::EndMenu();
        }
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

    ImGui::Image((void*)m_framebuffer->GetTexture(0), ImVec2(m_viewport_size.x, m_viewport_size.y), ImVec2(0, 1),
                 ImVec2(1, 0));

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
    if (m_hierarchy_window.GetSelected())
    {
        ImGuizmo::SetOrthographic(true);
        ImGuizmo::SetDrawlist();

        float window_height = (float)ImGui::GetWindowHeight();
        float window_width  = (float)ImGui::GetWindowWidth();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);

        // gizmo camera

        Leaper::OrthgraphicCamera& camera  = m_camera.GetCamera();
        const glm::mat4& camera_projection = camera.GetProjectionMat();
        glm::mat4 camera_view              = camera.GetViewMat();

        auto& tc            = m_hierarchy_window.GetSelected().GetComponent<Leaper::TransformComponent>();
        glm::mat4 transform = tc.GetTransform();

        if (Leaper::Input::IsKeyDown(LP_KEY_1))
            m_gizmo = ImGuizmo::OPERATION::TRANSLATE;
        if (Leaper::Input::IsKeyDown(LP_KEY_2))
            m_gizmo = ImGuizmo::OPERATION::ROTATE;
        if (Leaper::Input::IsKeyDown(LP_KEY_3))
            m_gizmo = ImGuizmo::OPERATION::SCALE;

        ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection),
                             (ImGuizmo::OPERATION)m_gizmo, ImGuizmo::LOCAL, glm::value_ptr(transform));

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

    // Output Log
    ImGui::Begin("Output Log");

    auto messages_          = Leaper::Log::GetSink()->log_item;
    const size_t max_shown_ = 1024;
    const ImColor TXT_DBG_CLR{ 0.5f, 0.5f, 0.5f, 1.0f };

    const ImColor LVL_DBG_CLR{ 0.0f, 1.0f, 0.8f, 1.0f };
    const ImColor LVL_LOG_CLR{ 0.65f, 0.2f, 1.0f, 1.0f };
    const ImColor LVL_ERR_CLR{ 1.0f, 0.0f, 0.0f, 1.0f };
    const ImColor LVL_WRN_CLR{ 1.0f, 0.5f, 0.0f, 1.0f };

    for (size_t i = messages_.size() > max_shown_ ? messages_.size() - max_shown_ : 0; i < messages_.size(); ++i)
    {
        auto const& msg = messages_[i];

        switch (msg.level)
        {
        case spdlog::level::debug:
            ImGui::TextColored(LVL_DBG_CLR, "[DEBUG]");
            ImGui::SameLine();
            ImGui::TextColored(LVL_DBG_CLR, "%s", msg.message.c_str());
            break;
        case spdlog::level::warn:
            ImGui::TextColored(LVL_WRN_CLR, "[WARING]");
            ImGui::SameLine();
            ImGui::Text("%s", msg.message.c_str());
            break;
        case spdlog::level::err:
            ImGui::TextColored(LVL_ERR_CLR, "[ERROR]");
            ImGui::SameLine();
            ImGui::Text("%s", msg.message.c_str());
            break;
        default:
            ImGui::TextColored(LVL_LOG_CLR, "[LOG]");
            ImGui::SameLine();
            ImGui::TextUnformatted(msg.message.c_str());
            break;
        }
    }
    ImGui::End();

    DrawToolBar();
    m_hierarchy_window.OnUpdate();
    m_project_window.OnUpdate();
}

void EditorLayer::OnEvent(Leaper::Event& e)
{
    m_camera.OnEvent(e);
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
    ImGui::Begin("##toolbar", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    // play icon
    Leaper::Ref<Leaper::Texture> button_icon = m_scene_state == SceneState::Edit ? m_play_icon : m_stop_icon;

    bool toolbarEnabled = (bool)m_active_scene;

    ImVec4 tint_color = ImVec4(1, 1, 1, 1);
    if (!toolbarEnabled)
        tint_color.w = 0.5f;
    float size = ImGui::GetWindowHeight() - 4.0f;

    // play button
    if (ImGui::ImageButton((void*)button_icon->GetTexture(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0,
                           ImVec4(0, 0, 0, 0), tint_color))
    {
        if (m_scene_state == SceneState::Edit)
            OnScenePlay();
        else if (m_scene_state == SceneState::Play)
            OnSceneEdit();
    }
    ImGui::SameLine();

    // CheckBox("DrawRect")
    ImGui::Checkbox("DrawRect", &m_draw_rect);
    ImGui::SameLine();

    // show fps
    ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);

    ImGui::PopStyleVar(2);
    ImGui::End();
}
