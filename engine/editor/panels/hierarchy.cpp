#include "core/math/math.h"
#include "function/application/application.h"
#include "function/ecs/components.h"
#include "hierarchy.h"
#include "imgui.h"
#include "project.h"

#include <ImGuizmo.h>
#include <imgui_internal.h>

#include <iostream>
#include <string>
#include <utility>

extern const std::filesystem::path g_assets_path;

Hierarchy::Hierarchy(const Leaper::Ref<Leaper::Scene>& scene)
{
    SetScene(scene);
}

void Hierarchy::OnAttach() {}

void Hierarchy::OnUpdate()
{
    // Hierarchy Window
    {
        ImGui::Begin("Hierarchy");

        m_active_scene->Reg().each([&](entt::entity id) {
            Leaper::Entity entity = { id, m_active_scene.get() };
            DrawEntityNode(entity);
        });

        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity")) m_active_scene->CreateEntity("Empty Entity");
            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Components");
        if (m_selected) DrawComponents(m_selected);
        ImGui::End();
    }
}
// mbb
void Hierarchy::DrawEntityNode(Leaper::Entity entity)
{
    // Get Component
    auto& tag = entity.GetComponent<Leaper::TagComponent>();
    // Set TreeNodeFlag
    ImGuiTreeNodeFlags flag =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ((m_selected == entity) ? ImGuiTreeNodeFlags_Selected : 0);

    bool create      = false;
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    bool opened      = ImGui::TreeNodeEx((void*)(uint32_t)(entity), flag, tag.Tag().c_str());
    // Determine whether the entity is destroyed
    bool destroy = false;

    if (ImGui::BeginPopupContextItem())
    {
        // Is destory entity;
        if (ImGui::Selectable("Delete Entity")) destroy = true;
        ImGui::EndPopup();
    }

    // Selected Entity
    if (ImGui::IsItemClicked())
    {
        m_selected = entity;
        LP_LOG("Selected");
    }

    if (opened) ImGui::TreePop();

    if (destroy)
    {
        // Destory entity
        m_active_scene->DestroyEntity(m_selected);
        if (m_selected == entity) { m_selected = {}; }
    }
}

template <typename T, typename... Args> void DrawComponentMenuItem(const std::string& name, Leaper::Entity entity, Args&&... args)
{
    if (ImGui::MenuItem(name.c_str()))
    {
        if (!entity.HasComponent<T>()) entity.AddComponent<T>(std::forward<Args>(args)...);
    }
}

void Hierarchy::DrawComponents(Leaper::Entity entity)
{
    if (entity.HasComponent<Leaper::TagComponent>() && entity.HasComponent<Leaper::UUIDComponent>())
    {
        ImGui::Text("UUID: %s", std::to_string(entity.GetComponent<Leaper::UUIDComponent>().uuid).c_str());

        auto& component = entity.GetComponent<Leaper::TagComponent>();
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy_s(buffer, sizeof(buffer), component.Tag().c_str(), sizeof(buffer));
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) { component.Tag() = std::string(buffer); }
    }
    ImGui::SameLine();
    float size = GImGui->Font->FontSize + GImGui->Style.FramePadding.y - 4.0f;

    // Add Component Button
    if (ImGui::Button("+")) ImGui::OpenPopup("Add Component Popup");

    if (ImGui::BeginPopupContextItem("Add Component Popup"))
    {
        DrawComponentMenuItem<Leaper::SpriteRendererComponent>("SpriteRenderer Component", entity);
        DrawComponentMenuItem<Leaper::Rigidbody2DComponent>("Rigidbody2D Component", entity);
        DrawComponentMenuItem<Leaper::BoxCollider2DComponent>("BoxCollider2D Component", entity);
        DrawComponentMenuItem<Leaper::CircleCollider2DComponent>("CircleCollider2D Component", entity);
        DrawComponentMenuItem<Leaper::LuaScriptComponent>("LuaScript Component", entity, " ");
        DrawComponentMenuItem<Leaper::Animation2DComponent>("Animation2D Component", entity);
        DrawComponentMenuItem<Leaper::SoundComponent>("Sound Component", entity);
        DrawComponentMenuItem<Leaper::LightComponent>("Light Component", entity);
        DrawComponentMenuItem<Leaper::MeshRendererComponment>("Mesh Renderer Component", entity);

        ImGui::EndPopup();
    }

    DrawComponent<Leaper::TransformComponent>("Transform Component", entity, [](auto& component) {
        DrawVector("Position", component.position, 0.01f);
        DrawVector("Rotation", component.rotation, 0.01f);
        DrawVector("Scale", component.scale, 0.01f);
    });

    DrawComponent<Leaper::CameraComponent>("Camera Component", entity, [](auto& component) {});

    DrawComponent<Leaper::SpriteRendererComponent>("SpriteRenderer Component", entity, [](auto& component) {
        ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

        ImGui::Button("Texture", ImVec2(ImGui::GetContentRegionAvail().x, 0));
        static std::filesystem::path texture_path;

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                texture_path        = std::filesystem::path(g_assets_path) / path;
                component.m_texture = Leaper::Texture::Create(texture_path.string());
            }

            ImGui::EndDragDropTarget();
        }
    });

    DrawComponent<Leaper::Rigidbody2DComponent>("Rigdbody2D Component", entity, [=](auto& component) {
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
                    component.body_type      = (Leaper::Rigidbody2DComponent::BodyType)i;
                }

                if (is_selected) ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
        ImGui::DragFloat2("Velocity", glm::value_ptr(component.velocity), 0.01f);
    });

    DrawComponent<Leaper::BoxCollider2DComponent>("BoxCollider2D Component", entity, [=](auto& component) {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy_s(buffer, sizeof(buffer), component.user_data->name.c_str(), sizeof(buffer));
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) { component.user_data->name = std::string(buffer); }

        ImGui::DragFloat2("Size", glm::value_ptr(component.size), 0.01f);
        ImGui::DragFloat("Frication", &component.friction, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Density", &component.density, 0.01f);
        ImGui::Checkbox("IsTrigger", &component.is_trigger);
    });

    DrawComponent<Leaper::CircleCollider2DComponent>("CircleCollider2D Component", entity, [=](auto& component) {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy_s(buffer, sizeof(buffer), component.user_data->name.c_str(), sizeof(buffer));
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) { component.user_data->name = std::string(buffer); }

        ImGui::DragFloat("Radiu", &component.radiu, 0.01f, 0.0f);
        ImGui::DragFloat("Frication", &component.friction, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Density", &component.density, 0.01f);
        ImGui::Checkbox("IsTrigger", &component.is_trigger);
    });

    DrawComponent<Leaper::LuaScriptComponent>("LuaScript Component", entity, [=](auto& component) {
        static std::filesystem::path script_path;
        std::string label = std::filesystem::path(component.path).stem().string();
        ImGui::Button(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                script_path         = std::filesystem::path(g_assets_path) / path;
                if (script_path.extension() == ".lua") { component.path = script_path.string(); }
                else { LP_CORE_LOG_ERROR("It isn't lua file"); }
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::SameLine();
        ImGui::Text("Script");
    });

    DrawComponent<Leaper::Animation2DComponent>("Animation2D Component", entity, [=](auto& component) {
        ImGui::DragInt("Col", &component.col, 1, 2);
        ImGui::DragInt("Row", &component.row, 1, 2);
        ImGui::DragInt("Speed", &component.speed, 1, 1);
    });

    DrawComponent<Leaper::SoundComponent>("Sound Component", entity, [=](auto& component) {
        static std::filesystem::path sound_path;

        std::string label = std::filesystem::path(component.path).stem().string();
        ImGui::Button(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                sound_path          = std::filesystem::path(g_assets_path) / path;
                component.path      = sound_path.string();
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::SameLine();
        ImGui::Text("Sound");
    });

    DrawComponent<Leaper::LightComponent>("Light Component", entity, [=](Leaper::LightComponent& component) {
        ImGui::ColorEdit4("Light Color", glm::value_ptr(component.color));
        ImGui::DragFloat("Intensity", &component.intensity);
    });

    DrawComponent<Leaper::MeshRendererComponment>("Mesh Renderer Component", entity, [=](Leaper::MeshRendererComponment& component) {
        static std::filesystem::path model_path;

        std::string label = std::filesystem::path(component.path).stem().string();
        ImGui::Button(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                model_path          = std::filesystem::path(g_assets_path) / path;
                component.path      = model_path.string();
                component.model     = Leaper::Model(component.path);
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::SameLine();
        ImGui::Text("Model");
    });
}

void Hierarchy::DrawVector(const std::string& label, glm::vec3& values, float speed)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushID(label.c_str());
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() / 1.15f);

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
    ImGui::SameLine();

    ImGui::Text(label.c_str());

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
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

template <typename T, typename DrawFunc> void Hierarchy::DrawComponent(const std::string& name, Leaper::Entity entity, DrawFunc func)
{

    const ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if (entity.HasComponent<T>())
    {
        auto& component = entity.GetComponent<T>();
        bool header     = ImGui::TreeNodeEx(name.c_str(), treeNodeFlags);
        ImGui::SameLine();

        if (ImGui::Button("~")) { ImGui::OpenPopup("ComponentSettings"); }

        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove Component")) entity.RemoveComponent<T>();
            ImGui::EndPopup();
        }

        if (header)
        {
            func(component);
            ImGui::TreePop();
        }
    }
}