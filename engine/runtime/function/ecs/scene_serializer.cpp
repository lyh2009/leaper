#include "scene_serializer.h"
#include "function/ecs/components.h"
#include "lppch.h"

#include <glm/glm.hpp>

#include <fstream>
#include <ostream>

// vec2

void to_json(nlohmann::json& j, const glm::vec2& t)
{
    j = nlohmann::json{ { "x", t.x }, { "y", t.y } };
}
void from_json(const nlohmann::json& j, glm::vec2& t)
{
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
}

// vec3
void to_json(nlohmann::json& j, const glm::vec3& t)
{
    j = nlohmann::json{ { "x", t.x }, { "y", t.y }, { "z", t.z } };
}
void from_json(const nlohmann::json& j, glm::vec3& t)
{
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
    j.at("z").get_to(t.z);
}

// vec4
void to_json(nlohmann::json& j, const glm::vec4& t)
{
    j = nlohmann::json{ { "x", t.x }, { "y", t.y }, { "z", t.z }, { "w", t.w } };
}
void from_json(const nlohmann::json& j, glm::vec4& t)
{
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
    j.at("z").get_to(t.z);
    j.at("w").get_to(t.w);
}

// TagComponent
void to_json(nlohmann::json& j, const Leaper::TagComponent& t)
{
    j = nlohmann::json{ { "TagComponent", t.tag } };
}
void from_json(const nlohmann::json& j, Leaper::TagComponent& t)
{
    j.at("TagComponent").get_to(t.tag);
}

// TransformComponent
void to_json(nlohmann::json& j, const Leaper::TransformComponent& t)
{
    nlohmann::json j_position;
    nlohmann::json j_rotation;
    nlohmann::json j_scale;
    to_json(j_position, t.position);
    to_json(j_rotation, t.rotation);
    to_json(j_scale, t.scale);
    j["TransformComponent"] = nlohmann::json{ { "position", j_position }, { "rotation", j_rotation }, { "scale", j_scale } };
}
void from_json(const nlohmann::json& j, Leaper::TransformComponent& t)
{
    from_json(j.at("TransformComponent").at("position"), t.position);
    from_json(j.at("TransformComponent").at("rotation"), t.rotation);
    from_json(j.at("TransformComponent").at("scale"), t.scale);
}

// Camera Component
void to_json(nlohmann::json& j, const Leaper::CameraComponent& t)
{
    j["CameraComponent"] = nlohmann::json{ { "ratio", t.camera.GetRatio() } };
}

// SpriteRendererComponent
void to_json(nlohmann::json& j, const Leaper::SpriteRendererComponent& t)
{
    nlohmann::json j_vec4;
    to_json(j_vec4, t.color);
    if (t.m_texture)
    {
        j["SpriteRendererComponent"] = nlohmann::json{ { "color", j_vec4 }, { "path", t.m_texture->GetPath() } };
    }
    else
    {
        j["SpriteRendererComponent"] = nlohmann::json{ { "color", j_vec4 } };
    }
}
void from_json(const nlohmann::json& j, Leaper::SpriteRendererComponent& t)
{
    from_json(j.at("SpriteRendererComponent").at("color"), t.color);
    if (j.at("SpriteRendererComponent").contains("path"))
        t.m_texture = Leaper::Texture::Create(j.at("SpriteRendererComponent").at("path"));
}

void to_json(nlohmann::json& j, const Leaper::Rigidbody2DComponent& t)
{
    j["Rigidbody2DComponent"] = nlohmann::json{ { "type", t.body_type } };
}
void from_json(const nlohmann::json& j, Leaper::Rigidbody2DComponent& t)
{
    j.at("Rigidbody2DComponent").at("type").get_to(t.body_type);
}

void to_json(nlohmann::json& j, const Leaper::BoxCollider2DComponent& t)
{
    nlohmann::json j_size;
    nlohmann::json j_offset;
    to_json(j_size, t.size);
    j["BoxCollider2DComponent"] = nlohmann::json{ { "tag", t.user_data->name },     { "size", j_size },       { "offset", j_offset },        { "friction", t.friction },
                                                  { "restitution", t.restitution }, { "density", t.density }, { "is_trigger", t.is_trigger } };
}
void from_json(const nlohmann::json& j, Leaper::BoxCollider2DComponent& t)
{
    j.at("BoxCollider2DComponent").at("tag").get_to(t.user_data->name);
    from_json(j.at("BoxCollider2DComponent").at("size"), t.size);
    j.at("BoxCollider2DComponent").at("friction").get_to(t.friction);
    j.at("BoxCollider2DComponent").at("restitution").get_to(t.restitution);
    j.at("BoxCollider2DComponent").at("density").get_to(t.density);
    j.at("BoxCollider2DComponent").at("is_trigger").get_to(t.is_trigger);
}

void to_json(nlohmann::json& j, const Leaper::LuaScriptComponent& t)
{
    j["LuaScriptComponent"] = nlohmann::json{ { "script", t.path } };
}

void from_json(const nlohmann::json& j, Leaper::LuaScriptComponent& t)
{
    j.at("LuaScriptComponent").at("script").get_to(t.path);
}

void to_json(nlohmann::json& j, const Leaper::UUIDComponent& t)
{
    j["UUIDComponent"] = nlohmann::json{ { "uuid", (uint64_t)t.uuid } };
}

void from_json(const nlohmann::json& j, Leaper::UUIDComponent& t)
{
    j.at("UUIDComponent").at("uuid").get_to(t.uuid.GetBase());
}

void to_json(nlohmann::json& j, const Leaper::SoundComponent& t)
{
    j["SoundComponent"] = nlohmann::json{ { "path", t.path.c_str() } };
}

void from_jon(const nlohmann::json& j, Leaper::SoundComponent& t)
{
    j.at("SoundComponent").at("path").get_to(t.path);
}

// Scene serializer
Leaper::SceneSerializer::SceneSerializer(Leaper::Ref<Leaper::Scene> scene) : m_scene(scene) {}

template <typename T, typename... Args> void ReadComponent(nlohmann::json& j, Leaper::Entity entity, const std::string& component, Args&&... args)
{
    if (j.contains(component))
    {
        entity.AddComponent<T>(std::forward<Args>(args)...);
        from_json(j, entity.GetComponent<T>());
    }
}

void Leaper::SceneSerializer::Read(const std::string& path)
{
    std::ifstream file(path);
    m_json = nlohmann::json::parse(file);

    for (auto& element : m_json)
    {
        Leaper::Entity entity;
        if (element.contains("TagComponent") && element.contains("UUIDComponent"))
        {
            entity = m_scene->CreateEntityWithUUID(element.at("TagComponent"), Leaper::UUID(element.at("UUIDComponent").at("uuid")));
        }

        if (element.contains("TransformComponent"))
        {
            Leaper::TransformComponent tc;
            from_json(element, tc);
            entity.GetComponent<Leaper::TransformComponent>() = tc;
        }

        if (element.contains("CameraComponent"))
        {
            float ratio;
            element.at("CameraComponent").at("ratio").get_to(ratio);
            entity.AddComponent<Leaper::CameraComponent>(ratio);
        }

        ReadComponent<Leaper::SpriteRendererComponent>(element, entity, "SpriteRendererComponent");
        ReadComponent<Leaper::Rigidbody2DComponent>(element, entity, "Rigidbody2DComponent");
        ReadComponent<Leaper::BoxCollider2DComponent>(element, entity, "BoxCollider2DComponent");
        ReadComponent<Leaper::LuaScriptComponent>(element, entity, "LuaScriptComponent");
        ReadComponent<Leaper::LuaScriptComponent>(element, entity, "SoundComponent");
    }
}

void Leaper::SceneSerializer::Write(const std::string& path)
{
    m_scene->Reg().each([&](auto entity_id) {
        Leaper::Entity entity = { entity_id, m_scene.get() };
        WriteEntity(entity);
    });

    std::ofstream file(path);
    file << m_json.dump(4);
}

template <typename T> void WriteComponent(nlohmann::json& j_entity, Leaper::Entity entity)
{
    if (entity.HasComponent<T>())
    {
        T& component = entity.GetComponent<T>();
        to_json(j_entity, component);
    }
}

void Leaper::SceneSerializer::WriteEntity(Leaper::Entity entity)
{
    nlohmann::json j_entity;

    WriteComponent<TagComponent>(j_entity, entity);
    WriteComponent<UUIDComponent>(j_entity, entity);
    WriteComponent<TransformComponent>(j_entity, entity);
    WriteComponent<CameraComponent>(j_entity, entity);
    WriteComponent<SpriteRendererComponent>(j_entity, entity);
    WriteComponent<Rigidbody2DComponent>(j_entity, entity);
    WriteComponent<BoxCollider2DComponent>(j_entity, entity);
    WriteComponent<LuaScriptComponent>(j_entity, entity);
    WriteComponent<SoundComponent>(j_entity, entity);

    m_json.push_back(j_entity);
}
