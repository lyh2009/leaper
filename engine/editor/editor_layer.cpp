#include <cstdint>
#include <function/render/renderer2d.h>

#include "editor_layer.h"

#include "ImGuizmo.h"
#include "core/base.h"
#include "core/log.h"
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

#include <IconsFontAwesome6.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui_internal.h>

#include <iostream>
#include <string>

#include "global.h"

namespace Leaper
{
    EditorLayer::EditorLayer() : Layer("EditorLayer"), m_camera(1024.0f / 648.0f), m_ortho_camera(1024 / 648) {}

    void EditorLayer::InitPanels(std::string_view assets_path)
    {
        m_assets_path = assets_path;
        m_hierarchy_window.OnAttach(assets_path);
        m_content_broswer_window.OnAttach(assets_path);
    }

    void EditorLayer::OnAttach()
    {

        // create FrameBuffer
        m_framebuffer = FrameBuffer::Create();
        m_framebuffer->CreateTexture(1920, 1080, TextureFormat::RGB8, TextureFormat::RGB, Attachments::COLOR_ATTACHMENT0);
        m_framebuffer->CreateTexture(1920, 1080, TextureFormat::R32I, TextureFormat::RED_INTEGER, Attachments::COLOR_ATTACHMENT1);
        m_framebuffer->CreateDepthTexture(1920, 1080);
        // create Textures
        m_play_icon      = Texture::Create("./resource/icons/play.png");
        m_stop_icon      = Texture::Create("./resource/icons/stop.png");
        m_translate_icon = Texture::Create("./resource/icons/translate.png");
        m_rotate_icon    = Texture::Create("./resource/icons/rotate.png");
        m_scale_icon     = Texture::Create("./resource/icons/scale.png");

        m_active_scene = CreateRef<Scene>();
        m_active_scene->OnAttach();

        camera_entity = m_active_scene->CreateEntity("camera");
        camera_entity.AddComponent<CameraComponent>(1024.0f / 648.0f);
        camera_entity.AddComponent<CubeRendererComponent>();

        m_console.OnAttach();

        m_perspective_camera = EditorCamera(30.0f, 1024 / 648, 0.1f, 1000.0f);
    }

    void EditorLayer::OnUpdate()
    {

        RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        RenderCommand::Clear();
        m_framebuffer->Bind();
        RenderCommand::SetMainFrameBuffer(m_framebuffer);
        m_framebuffer->ClearAttachment(1, -1);
        // Renderer3D::SetFrameBuffer(m_framebuffer);

        // resize window
        if (m_viewport_size != *((glm::vec2*)&m_viewport_panel_size))
        {
            m_camera.OnResize(m_viewport_panel_size.x, m_viewport_panel_size.y);
            m_ortho_camera.OnResize(m_viewport_panel_size.x, m_viewport_panel_size.y);
            m_perspective_camera.SetViewportSize(m_viewport_panel_size.x, m_viewport_panel_size.y);
            m_viewport_size = { m_viewport_panel_size.x, m_viewport_panel_size.y };
            camera_entity.GetComponent<CameraComponent>().camera.OnResize(m_viewport_panel_size.x, m_viewport_panel_size.y);

            m_framebuffer->RescaleFrameBuffer(m_viewport_panel_size.x, m_viewport_panel_size.y);
            // Renderer3D::s_frame_buffer->RescaleFrameBuffer(m_viewport_panel_size.x, m_viewport_panel_size.y);
        }

        // scene update
        m_ortho_camera     = camera_entity.GetComponent<CameraComponent>().camera;
        auto& camera_trans = camera_entity.GetComponent<TransformComponent>();

        if (m_scene_state == SceneState::Edit)
        {
            if (m_editor_camera_type == CameraTypes::Ortho)
            {
                m_camera.OnUpdate();
                auto pos = m_camera.GetCamera().GetPosition();
                Renderer2D::BeginScene(m_camera.GetCamera().GetViewContentBroswerionMat());
            }
            else
            {
                m_perspective_camera.OnUpdate();

                Renderer2D::BeginScene(m_perspective_camera.GetViewContentBroswerion(), m_perspective_camera.GetPosition());
                Renderer3D::SetCamera(m_perspective_camera.GetViewMatrix(), m_perspective_camera.GetContentBroswerion(), m_perspective_camera.GetPosition());
                Renderer3D::BeginScene(m_perspective_camera.GetViewContentBroswerion(), m_perspective_camera.GetPosition());
            }
        }
        else if (m_scene_state == SceneState::Play)
        {
            m_ortho_camera.OnUpdate();
            Renderer2D::BeginScene(m_ortho_camera.GetCamera().GetContentBroswerionMat(), camera_trans.GetTransform());
        }

        RenderCommand::SetLightSpaceMat(m_perspective_camera.GetContentBroswerion());
        m_active_scene->OnUpdate();

        // render rect
        if (m_draw_rect)
        {
            Renderer2D::SetLineWidth(1.0f);
            TransformComponent transform;
            float ratio        = m_ortho_camera.GetRatio();
            float zoom_level   = m_ortho_camera.GetZoomLevel();
            transform.scale    = glm::vec3(ratio * zoom_level * 2, zoom_level * 2, 1.0f);
            transform.position = camera_trans.position;
            transform.rotation = camera_trans.rotation;
            Renderer2D::DrawRect(transform, glm::vec4(1.0, 1.0, 1.0, 1), -1);

            auto view = m_active_scene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
            for (auto entity : view)
            {
                auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

                TransformComponent transform;
                transform.position = tc.position;
                transform.scale    = tc.scale * glm::vec3(bc2d.size * 2.0f, 1.0f);
                transform.rotation = tc.rotation;

                Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
            }

            auto cc2d_view = m_active_scene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
            for (auto entity : cc2d_view)
            {
                auto [tc, cc2d] = cc2d_view.get<TransformComponent, CircleCollider2DComponent>(entity);

                TransformComponent transform;
                transform.position = tc.position;
                transform.scale    = tc.scale * cc2d.radiu;
                transform.rotation = tc.rotation;

                Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.05f);
            }
        }
        Renderer2D::EndScene();
        Renderer3D::EndScene();

        // get entity
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_viewport_bounds[0].x;
        my -= m_viewport_bounds[0].y;
        glm::vec2 viewport_size = m_viewport_bounds[1] - m_viewport_bounds[0];
        my                      = viewport_size.y - my;
        int mouseX              = (int)mx;
        int mouseY              = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewport_size.x && mouseY < (int)viewport_size.y)
        {
            int entity_id    = m_framebuffer->ReadPixels(1, (int)mouseX, (int)mouseY);
            m_hovered_entity = entity_id == -1 ? Entity() : Entity((entt::entity)entity_id, m_active_scene.get());
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
                    std::string filepath = FileDialogs::SaveFile("Leaper Scene(*.leaper)\0*.leaper\0");
                    if (!filepath.empty())
                    {
                        SceneSerializer serizlizer(m_active_scene);
                        serizlizer.Write(filepath + ".leaper");
                    }
                }

                if (ImGui::MenuItem("Open..."))
                {
                    std::string filepath = FileDialogs::OpenFile("Leaper Scene(*.leaper)\0*.leaper\0");
                    if (!filepath.empty())
                    {
                        m_hierarchy_window.GetSelected() = {};

                        m_active_scene = CreateRef<Scene>();
                        m_hierarchy_window.SetScene(m_active_scene);
                        SceneSerializer serizlizer(m_active_scene);
                        serizlizer.Read(filepath);
                        m_active_scene->OnAttach();
                        auto view = m_active_scene->Reg().view<CameraComponent>();
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
        ImGui::Begin(ICON_FA_DESKTOP " Scene");
        ImGui::SameLine();
        const char* camera_types_string[] = { "Orthographic", "Perspective" };
        const char* current_camera_type   = camera_types_string[m_editor_camera_type];
        if (ImGui::BeginCombo("CameraTypes", current_camera_type))
        {
            for (int i = 0; i < 2; i++)
            {
                bool is_selected = current_camera_type == camera_types_string[i];
                if (ImGui::Selectable(camera_types_string[i], is_selected))
                {
                    current_camera_type  = camera_types_string[i];
                    m_editor_camera_type = (CameraTypes)i;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::BeginChild("Viewport");

        auto viewport_min_region = ImGui::GetWindowContentRegionMin();
        auto viewport_max_region = ImGui::GetWindowContentRegionMax();
        auto viewport_offset     = ImGui::GetWindowPos();
        m_viewport_bounds[0]     = { viewport_min_region.x + viewport_offset.x, viewport_min_region.y + viewport_offset.y };
        m_viewport_bounds[1]     = { viewport_max_region.x + viewport_offset.x, viewport_max_region.y + viewport_offset.y };

        m_viewport_hovered    = ImGui::IsWindowHovered();
        m_viewport_panel_size = ImGui::GetContentRegionAvail();

        ImGui::Image(reinterpret_cast<void*>(m_framebuffer->GetTexture(0)), ImVec2(m_viewport_panel_size.x, m_viewport_panel_size.y), ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
            {
                const wchar_t* path              = (const wchar_t*)payload->Data;
                std::filesystem::path filepath   = std::filesystem::path(m_assets_path) / path;
                m_hierarchy_window.GetSelected() = {};

                m_active_scene = CreateRef<Scene>();
                m_hierarchy_window.SetScene(m_active_scene);
                SceneSerializer serizlizer(m_active_scene);
                serizlizer.Read(filepath.string());
                m_active_scene->OnAttach();
                auto view = m_active_scene->Reg().view<CameraComponent>();
                for (auto e : view) { camera_entity = { e, m_active_scene.get() }; }
            }

            ImGui::EndDragDropTarget();
        }

        // gizmo
        auto select = m_hierarchy_window.GetSelected();
        if (select && m_gizmo != -1)
        {
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_viewport_bounds[0].x, m_viewport_bounds[0].y, m_viewport_bounds[1].x - m_viewport_bounds[0].x, m_viewport_bounds[1].y - m_viewport_bounds[0].y);

            // gizmo camera

            auto& tc            = select.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();
            if (m_scene_state == SceneState::Edit)
            {
                if (m_editor_camera_type == CameraTypes::Ortho)
                {
                    ImGuizmo::SetOrthographic(true);
                    OrthgraphicCamera& camera                 = m_camera.GetCamera();
                    const glm::mat4& camera_ContentBroswerion = camera.GetContentBroswerionMat();
                    glm::mat4 camera_view                     = camera.GetViewMat();

                    ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_ContentBroswerion), (ImGuizmo::OPERATION)m_gizmo, ImGuizmo::LOCAL,
                                         glm::value_ptr(transform), nullptr, nullptr, nullptr, nullptr);
                }

                else
                {
                    ImGuizmo::SetOrthographic(false);
                    const glm::mat4& camera_ContentBroswerion = m_perspective_camera.GetContentBroswerion();
                    glm::mat4 camera_view                     = m_perspective_camera.GetViewMatrix();

                    ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_ContentBroswerion), (ImGuizmo::OPERATION)m_gizmo, ImGuizmo::LOCAL,
                                         glm::value_ptr(transform), nullptr, nullptr, nullptr, nullptr);
                }
            }
            else
            {
                OrthgraphicCamera& camera                 = m_ortho_camera.GetCamera();
                const glm::mat4& camera_ContentBroswerion = camera.GetContentBroswerionMat();
                glm::mat4 camera_view                     = camera.GetViewMat();

                ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_ContentBroswerion), (ImGuizmo::OPERATION)m_gizmo, ImGuizmo::LOCAL,
                                     glm::value_ptr(transform), nullptr, nullptr, nullptr, nullptr);
            }

            if (Input::IsKeyDown(LP_KEY_1))
                m_gizmo = ImGuizmo::OPERATION::TRANSLATE;
            if (Input::IsKeyDown(LP_KEY_2))
                m_gizmo = ImGuizmo::OPERATION::ROTATE;
            if (Input::IsKeyDown(LP_KEY_3))
                m_gizmo = ImGuizmo::OPERATION::SCALE;

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 position, rotation, scale;
                Math::DecomposeTransform(transform, position, rotation, scale);
                glm::vec3 delat_rotation = rotation - tc.rotation;
                tc.position              = position;
                tc.rotation += delat_rotation;
                tc.scale = scale;
            }
        }

        ImGui::EndChild();
        ImGui::End();

        ImGui::Begin("Debug");
        // ImGui::Image(reinterpret_cast<void*>(Renderer3D::s_frame_buffer->GetDepthTexture()), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::Text(std::to_string((uint32_t)m_hovered_entity).c_str());
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Text("%.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Checkbox("DrawRect", &m_draw_rect);
        ImGui::End();

        DrawToolBar();
        m_hierarchy_window.OnImGuiRender();
        m_content_broswer_window.OnImGuiRender();
        m_console.OnImGuiRender();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_camera.OnEvent(e);
        m_perspective_camera.OnEvent(e);
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(LP_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == Mosue::ButtonLeft)
        {
            if (m_viewport_hovered && !ImGuizmo::IsOver() && m_hovered_entity)
            {
                LP_CORE_LOG(std::to_string((uint32_t)m_hovered_entity));
                m_hierarchy_window.GetSelected() = m_hovered_entity;
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
        Ref<Texture> button_icon = m_scene_state == SceneState::Edit ? m_play_icon : m_stop_icon;

        bool toolbarEnabled = (bool)m_active_scene;

        ImVec4 tint_color = ImVec4(1, 1, 1, 1);
        if (!toolbarEnabled)
            tint_color.w = 0.5f;
        float size = ImGui::GetWindowHeight() - 4.0f;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        ImVec4 translate_color = m_gizmo == ImGuizmo::OPERATION::TRANSLATE ? ImVec4(1.0f, 1.0f, 1.0f, 0.9f) : ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
        ImVec4 rotate_color    = m_gizmo == ImGuizmo::OPERATION::ROTATE ? ImVec4(1.0f, 1.0f, 1.0f, 0.9f) : ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
        ImVec4 scale_color     = m_gizmo == ImGuizmo::OPERATION::SCALE ? ImVec4(1.0f, 1.0f, 1.0f, 0.9f) : ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
        if (ImGui::ImageButton((void*)m_translate_icon->GetTexture(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), translate_color))
        {
            m_gizmo = ImGuizmo::OPERATION::TRANSLATE;
        }
        ImGui::SameLine();
        if (ImGui::ImageButton(reinterpret_cast<void*>(m_rotate_icon->GetTexture()), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                               rotate_color))
        {
            m_gizmo = ImGuizmo::OPERATION::ROTATE;
        }
        ImGui::SameLine();
        if (ImGui::ImageButton(reinterpret_cast<void*>(m_scale_icon->GetTexture()), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                               scale_color))
        {
            m_gizmo = ImGuizmo::OPERATION::SCALE;
        }
        ImGui::SameLine();

        // play button
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

        if (ImGui::ImageButton((void*)button_icon->GetTexture(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tint_color))
        {
            if (m_scene_state == SceneState::Edit)
                OnScenePlay();
            else if (m_scene_state == SceneState::Play)
                OnSceneEdit();
        }
        ImGui::PopStyleColor();

        ImGui::PopStyleVar(2);
        ImGui::End();
    }
}  // namespace Leaper