#include "content_broswer.h"
#include "core/log.h"
#include "core/math/math.h"
#include "function/application/application.h"
#include "function/ecs/components.h"
#include "function/imgui/ui.h"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy.h"
#include "imgui.h"
#include "resource/gpu_resource_mapper.h"
#include <IconsFontAwesome6.h>

#include <ImGuizmo.h>
#include <cstdint>
#include <imgui_internal.h>

#include "../global.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace Leaper
{

    Hierarchy::Hierarchy(const Ref<Scene>& scene)
    {
        SetScene(scene);
    }

    void Hierarchy::OnAttach(std::string_view assets_path)
    {
        m_assets_path = assets_path;
    }

    void Hierarchy::OnImGuiRender()
    {
        // Hierarchy

        ImGui::Begin(ICON_FA_LIST " Hierarchy");

        m_active_scene->Reg().each([&](entt::entity id) {
            Entity entity = { id, m_active_scene.get() };
            DrawEntityNode(entity);
        });

        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                m_active_scene->CreateEntity("Empty Entity");
            ImGui::EndPopup();
        }

        ImGui::End();

        // Inspector
        ImGui::Begin(ICON_FA_CIRCLE_INFO " Components");
        if (m_selected)
            DrawComponents(m_selected);
        ImGui::End();
    }
    // mbb
    void Hierarchy::DrawEntityNode(Entity entity)
    {
        // Get Component
        auto& tag = entity.GetComponent<TagComponent>();
        // Set TreeNodeFlag
        ImGuiTreeNodeFlags flag =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ((m_selected == entity) ? ImGuiTreeNodeFlags_Selected : 0);

        bool create      = false;
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        bool opened      = ImGui::TreeNodeEx((void*)(uint32_t)(entity), flag, tag.tag.c_str());
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
            {
                auto entity_id  = (UUIDComponent*)payload->Data;
                auto new_entity = m_active_scene->GetEntityByUUID(entity_id->uuid);
                auto p_entity   = new_entity.AddComponent<ParentEntity>(entity);

                LP_CORE_INFO("Entity Tag:{0}", new_entity.GetComponent<TagComponent>().tag);
            }

            ImGui::EndDragDropTarget();
        }
        if (ImGui::BeginDragDropSource())
        {
            auto entity_id   = entity.GetComponent<UUIDComponent>();
            auto entity_name = entity.GetComponent<TagComponent>().tag;
            ImGui::SetDragDropPayload("ENTITY", &entity_id, sizeof(UUIDComponent));
            ImGui::Text(entity_name.c_str());
            ImGui::EndDragDropSource();
        }
        // Determine whether the entity is destroyed
        bool destroy = false;

        if (ImGui::BeginPopupContextItem())
        {
            // Is destory entity;
            if (ImGui::Selectable("Delete Entity"))
                destroy = true;
            ImGui::EndPopup();
        }

        // Selected Entity
        if (ImGui::IsItemClicked())
            m_selected = entity;

        if (opened)
            ImGui::TreePop();

        if (destroy)
        {
            // Destory entity
            m_active_scene->DestroyEntity(m_selected);
            if (m_selected == entity)
            {
                m_selected = {};
            }
        }
    }

    template <typename T, typename... Args> void DrawComponentMenuItem(const std::string& name, Entity entity, Args&&... args)
    {
        if (ImGui::MenuItem(name.c_str()))
        {
            if (!entity.HasComponent<T>())
                entity.AddComponent<T>(std::forward<Args>(args)...);
        }
    }

    void BeginColumn()
    {
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 100.0f);
    }

    void EndColumn()
    {
        ImGui::Columns(1);
    }

    void Hierarchy::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>() && entity.HasComponent<UUIDComponent>())
        {
            ImGui::Text("UUID: %s", std::to_string(entity.GetComponent<UUIDComponent>().uuid).c_str());

            auto& component = entity.GetComponent<TagComponent>();
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strncpy_s(buffer, sizeof(buffer), component.Tag().c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                component.Tag() = std::string(buffer);
            }
        }
        ImGui::SameLine();
        float size = GImGui->Font->FontSize + GImGui->Style.FramePadding.y - 4.0f;

        // Add Component Button
        if (ImGui::Button("+"))
            ImGui::OpenPopup("Add Component Popup");

        if (ImGui::BeginPopupContextItem("Add Component Popup"))
        {
            DrawComponentMenuItem<SpriteRendererComponent>(ICON_FA_BRUSH "SpriteRenderer Component", entity);
            if (ImGui::BeginMenu("Physics2D"))
            {
                DrawComponentMenuItem<Rigidbody2DComponent>(ICON_FA_RADIATION "Rigidbody2D Component", entity);
                DrawComponentMenuItem<BoxCollider2DComponent>(ICON_FA_VECTOR_SQUARE "BoxCollider2D Component", entity);
                DrawComponentMenuItem<CircleCollider2DComponent>(ICON_FA_CIRCLE_NOTCH "CircleCollider2D Component", entity);
                ImGui::EndMenu();
            }
            DrawComponentMenuItem<LuaScriptComponent>(ICON_FA_FILE_CODE "LuaScript Component", entity, " ");
            DrawComponentMenuItem<Animation2DComponent>(ICON_FA_BABY "Animation2D Component", entity);
            DrawComponentMenuItem<SoundComponent>(ICON_FA_MUSIC "Sound Component", entity);
            DrawComponentMenuItem<LightComponent>(ICON_FA_LIGHTBULB "Light Component", entity);
            DrawComponentMenuItem<MeshRendererComponment>(ICON_FA_DICE_D20 "Mesh Renderer Component", entity);
            DrawComponentMenuItem<DirectionalLightComponent>(ICON_FA_SUN "Directional Light Component", entity);
            DrawComponentMenuItem<PointLightComponent>(ICON_FA_LIGHTBULB "Point Light Component", entity);

            ImGui::EndPopup();
        }

        DrawComponent<TransformComponent>(ICON_FA_CROP " Transform Component", entity, [](auto& component) {
            DrawVector("Position", component.position, 0.01f);
            DrawVector("Rotation", component.rotation, 0.01f);
            DrawVector("Scale", component.scale, 0.01f);
        });

        DrawComponent<CameraComponent>(ICON_FA_VIDEO " Camera Component", entity, [](auto& component) {});

        DrawComponent<SpriteRendererComponent>(ICON_FA_BRUSH " SpriteRenderer Component", entity, [=](auto& component) {
            static std::filesystem::path texture_path;
            std::string label = std::filesystem::path(component.texture_path).stem().string();
            UI::BeginColumns();
            {
                UI::ColorEdit4("Color", glm::value_ptr(component.color));
                UI::Button("Texture", label, ImVec2(-1, 0));
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
                    {
                        const wchar_t* path    = (const wchar_t*)payload->Data;
                        texture_path           = m_assets_path / path;
                        component.texture_path = texture_path.string();
                        component.m_texture    = TextureResourceManager::LoadTexture(texture_path.string());
                    }

                    ImGui::EndDragDropTarget();
                }
            }
            UI::EndColumns();
        });

        DrawComponent<Rigidbody2DComponent>(ICON_FA_RADIATION " Rigdbody2D Component", entity, [=](auto& component) {
            const char* body_type_strings[]      = { "Static", "Dynamic", "Kinematic" };
            const char* current_body_type_string = body_type_strings[(int)component.body_type];
            if (ImGui::BeginCombo("Body Type", current_body_type_string))
            {
                for (int i = 0; i < 3; i++)
                {
                    bool is_selected = current_body_type_string == body_type_strings[i];
                    if (ImGui::Selectable(body_type_strings[i], is_selected))
                    {
                        current_body_type_string = body_type_strings[i];
                        component.body_type      = (Rigidbody2DComponent::BodyType)i;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
            UI::BeginColumns();
            {
                UI::DragFloat2("Velocity", glm::value_ptr(component.velocity), 0.01f);
                UI::DragFloat("GravityScale", &component.gravity_scale);
            }
            UI::EndColumns();
        });

        DrawComponent<BoxCollider2DComponent>(ICON_FA_VECTOR_SQUARE " BoxCollider2D Component", entity, [=](auto& component) {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strncpy_s(buffer, sizeof(buffer), component.user_data->name.c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                component.user_data->name = std::string(buffer);
            }
            UI::BeginColumns();
            {

                UI::DragFloat2("Size", glm::value_ptr(component.size), 0.01f);
                UI::DragFloat("Frication", &component.friction, 0.01f, 0.0f, 1.0f);
                UI::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
                UI::DragFloat("Density", &component.density, 0.01f);
                UI::Checkbox("IsTrigger", &component.is_trigger);
            }
            UI::EndColumns();
        });

        DrawComponent<CircleCollider2DComponent>(ICON_FA_CIRCLE_NOTCH "CircleCollider2D Component", entity, [=](auto& component) {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strncpy_s(buffer, sizeof(buffer), component.user_data->name.c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                component.user_data->name = std::string(buffer);
            }
            UI::BeginColumns();
            {
                UI::DragFloat("Radiu", &component.radiu, 0.01f, 0.0f);
                UI::DragFloat("Frication", &component.friction, 0.01f, 0.0f, 1.0f);
                UI::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
                UI::DragFloat("Density", &component.density, 0.01f);
                UI::Checkbox("IsTrigger", &component.is_trigger);
            }
            UI::EndColumns();
        });

        DrawComponent<LuaScriptComponent>(ICON_FA_FILE_CODE " LuaScript Component", entity, [=](auto& component) {
            static std::filesystem::path script_path;
            std::string label = std::filesystem::path(component.path).stem().string();
            UI::BeginColumns();
            {
                UI::Button("Script", label, ImVec2(-1, 0));
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
                    {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        script_path         = m_assets_path / path;
                        if (script_path.extension() == ".lua")
                        {
                            component.path = script_path.string();
                        }
                        else
                        {
                            LP_CORE_ERROR("It isn't lua file");
                        }
                    }

                    ImGui::EndDragDropTarget();
                }
                std::unordered_map<std::string, int>::iterator iter;
                for (iter = component.int_values.begin(); iter != component.int_values.end(); ++iter) { UI::DragInt(iter->first, &iter->second); }
            }

            UI::EndColumns();
        });

        DrawComponent<Animation2DComponent>(ICON_FA_BABY " Animation2D Component", entity, [=](auto& component) {
            ImGui::DragInt("Col", &component.col, 1, 2);
            ImGui::DragInt("Row", &component.row, 1, 2);
            ImGui::DragInt("Speed", &component.speed, 1, 1);
        });

        DrawComponent<SoundComponent>(ICON_FA_MUSIC " Sound Component", entity, [=](auto& component) {
            static std::filesystem::path sound_path;

            std::string label = std::filesystem::path(component.path).stem().string();
            UI::BeginColumns();
            {
                UI::Button("Sound", label, ImVec2(-1, 0));
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
                    {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        sound_path          = m_assets_path / path;
                        component.path      = sound_path.string();
                    }

                    ImGui::EndDragDropTarget();
                }
            }
            UI::EndColumns();
        });

        DrawComponent<LightComponent>(ICON_FA_LIGHTBULB " Light Component", entity, [=](LightComponent& component) {
            UI::BeginColumns();
            {
                UI::ColorEdit4("Light Color", glm::value_ptr(component.color));
                UI::DragFloat("Intensity", &component.intensity);
            }
            UI::EndColumns();
        });

        DrawComponent<MeshRendererComponment>(ICON_FA_DICE_D20 " Mesh Renderer Component", entity, [=](MeshRendererComponment& component) {
            static std::filesystem::path model_path;

            std::string label = std::filesystem::path(component.path).stem().string();
            UI::BeginColumns();
            {
                UI::Button("Model", label, ImVec2(-1, 0));
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
                    {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        model_path          = m_assets_path / path;
                        component.path      = model_path.string();
                        component.model     = ModelResourceManager::LoadModel(model_path.string());
                    }

                    ImGui::EndDragDropTarget();
                }
            }
            UI::EndColumns();
        });

        DrawComponent<DirectionalLightComponent>(ICON_FA_SUN "Directional Light Component", entity, [=](auto& component) {
            UI::BeginColumns();
            {
                UI::ColorEdit3("Light Color", glm::value_ptr(component.color));
                UI::DragFloat("Ambient Strength", &component.ambient_strength, 0.01f, 0.0f, 1.0f);
                UI::DragFloat("Specular Strength", &component.specular_strength, 0.01f, 0.0f, 1.0f);
            }
            UI::EndColumns();
        });

        DrawComponent<PointLightComponent>(ICON_FA_LIGHTBULB "Point Light Component", entity, [=](auto& component) {
            UI::BeginColumns();
            {
                UI::DragFloat("Constant", &component.constant, 0.01f, 0.0f, 1.0f);
                UI::DragFloat("Linear", &component.linear, 0.01f, 0.0f, 1.0f);
                UI::DragFloat("Quadratic", &component.quadratic, 0.01f, 0.0f, 1.0f);
            }
            UI::EndColumns();
        });
    }

    void Hierarchy::DrawVector(const std::string& label, glm::vec3& values, float speed)
    {
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 85.0f);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushID(label.c_str());
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::DragFloat("##x", &values.x, speed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        ImGui::DragFloat("##y", &values.y, speed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
        ImGui::DragFloat("##z", &values.z, speed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }

    const glm::vec2 Hierarchy::GetMousePosInImguiWindow()
    {
        ImVec2 mouse_pos            = ImGui::GetMousePos();
        ImVec2 window_size          = ImGui::GetWindowSize();
        ImVec2 window_pos           = ImGui::GetWindowPos();
        ImVec2 mouse_pos_in_window  = ImVec2(mouse_pos.x - window_pos.x, mouse_pos.y - window_pos.y);
        ImVec2 mouse_pos_in_texture = ImVec2(mouse_pos_in_window.x, window_size.y - mouse_pos_in_window.y);
        return glm::vec2(mouse_pos_in_texture.x, mouse_pos_in_texture.y);
    }

    template <typename T, typename DrawFunc> void Hierarchy::DrawComponent(const std::string& name, Entity entity, DrawFunc func)
    {

        const ImGuiTreeNodeFlags treeNodeFlags =
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();
            bool header     = ImGui::TreeNodeEx(name.c_str(), treeNodeFlags);
            ImGui::SameLine();

            if (ImGui::Button("~"))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                    entity.RemoveComponent<T>();
                ImGui::EndPopup();
            }

            if (header)
            {
                func(component);
                ImGui::TreePop();
            }
        }
    }
}  // namespace Leaper