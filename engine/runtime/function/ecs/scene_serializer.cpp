#include "scene_serializer.h"
#include "lppch.h"

#include <glm/glm.hpp>

#include <fstream>
#include <ostream>

namespace glm
{
    // vec2
    void to_json(nlohmann::json& j, const glm::vec2& t)
    {
        j = nlohmann::json{{"x", t.x}, {"y", t.y}};
    }
    void from_json(const nlohmann::json& j, glm::vec2& t)
    {
        j.at("x").get_to(t.x);
        j.at("y").get_to(t.y);
    }

    // vec3
    void to_json(nlohmann::json& j, const glm::vec3& t)
    {
        j = nlohmann::json{{"x", t.x}, {"y", t.y}, {"z", t.z}};
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
        j = nlohmann::json{{"x", t.x}, {"y", t.y}, {"z", t.z}, {"w", t.w}};
    }
    void from_json(const nlohmann::json& j, glm::vec4& t)
    {
        j.at("x").get_to(t.x);
        j.at("y").get_to(t.y);
        j.at("z").get_to(t.z);
        j.at("w").get_to(t.w);
    }
}

namespace Leaper
{
    //TagComponent
    void to_json(nlohmann::json& j, const Leaper::TagComponent& t)
    {
        j = nlohmann::json{{"tag", t.tag}};
    }
    void from_json(const nlohmann::json& j, Leaper::TagComponent& t)
    {
        j.at("tag").get_to(t.tag);
    }

    //TransformComponent
    void to_json(nlohmann::json& j, const Leaper::TransformComponent& t)
    {
        nlohmann::json j_position;
        nlohmann::json j_rotation;
        nlohmann::json j_scale;
        glm::to_json(j_position, t.position);
        glm::to_json(j_rotation, t.rotation);
        glm::to_json(j_scale, t.scale);
        j["transform_component"] = nlohmann::json
        {
            {"position", j_position}, 
            {"rotation", j_rotation}, 
            {"scale", j_scale}
        };
    }
    void from_json(const nlohmann::json& j, Leaper::TransformComponent& t)
    {
        glm::from_json(j.at("transform_component").at("position"), t.position);
        glm::from_json(j.at("transform_component").at("rotation"), t.rotation);
        glm::from_json(j.at("transform_component").at("scale"), t.scale);
    }

    //SpriteRendererComponent
    void to_json(nlohmann::json& j, const Leaper::SpriteRendererComponent& t)
    {
        nlohmann::json j_vec4;
        glm::to_json(j_vec4, t.color);
        if(t.m_texture)
        {
            j["sprite_renderer_component"] = nlohmann::json
            {
                {"color", j_vec4},
                {"path", t.m_texture->GetPath()}
            };

        }else
        {
            j["sprite_renderer_component"] = nlohmann::json
            {
                {"color", j_vec4}
            };
        }
    }
    void from_json(const nlohmann::json& j, Leaper::SpriteRendererComponent& t)
    {
        glm::from_json(j.at("sprite_renderer_component").at("color"), t.color);
        if(j.at("sprite_renderer_component").contains("path"))
            t.m_texture = Leaper::Texture::Create(j.at("sprite_renderer_component").at("path"));
    }

    void to_json(nlohmann::json& j, const Leaper::Rigidbody2DComponent& t)
    {
        j["rigidbody2d_component"] = nlohmann::json{{"type", t.body_type}};
    }
    void from_json(const nlohmann::json& j, Leaper::Rigidbody2DComponent& t)
    {
        j.at("rigidbody2d_component").at("type").get_to(t.body_type);
    }

    void to_json(nlohmann::json& j, const Leaper::BoxCollider2DComponent& t)
    {
        nlohmann::json j_size;
        nlohmann::json j_offset;
        glm::to_json(j_size, t.size);
        j["box_collider2d_component"] = nlohmann::json
        {
            {"size", j_size},
            {"offset", j_offset},
            {"friction", t.friction},
            {"restitution", t.restitution},
            {"density", t.density},
            {"is_trigger", t.is_trigger}
        };
    }
    void from_json(const nlohmann::json& j, Leaper::BoxCollider2DComponent& t)
    {
        glm::from_json(j.at("box_collider2d_component").at("size"), t.size);
        j.at("box_collider2d_component").at("friction").get_to(t.friction);
        j.at("box_collider2d_component").at("restitution").get_to(t.restitution);
        j.at("box_collider2d_component").at("density").get_to(t.density);
        j.at("box_collider2d_component").at("is_trigger").get_to(t.is_trigger);
    }

    void to_json(nlohmann::json& j, const Leaper::LuaScriptComponent& t)
    {
        j["lua_script_component"] = nlohmann::json{{"script", t.path}};
    }

    void from_json(const nlohmann::json& j, Leaper::LuaScriptComponent& t)
    {
        j.at("lua_script_component").at("script").get_to(t.path);
    }

    void to_json(nlohmann::json& j, const Leaper::UUIDComponent& t)
    {
        j["uuid_component"] = nlohmann::json{{"uuid", (uint64_t)t.uuid}};
    }

    void from_json(const nlohmann::json& j, Leaper::UUIDComponent& t)
    {
        j.at("uuid_component").at("uuid").get_to(t.uuid.GetBase());
    }
}

Leaper::SceneSerializer::SceneSerializer(Leaper::Ref<Leaper::Scene> scene)
    : m_scene(scene)
{

}

void Leaper::SceneSerializer::Read(const std::string& path)
{
    std::ifstream file(path);
    m_json = nlohmann::json::parse(file);

    for(auto& element : m_json.at("entities"))
    {
        Leaper::Entity entity;
        if(element.contains("tag") && element.contains("uuid_component"))
        {
            entity = m_scene->CreateEntityWithUUID(element.at("tag"), Leaper::UUID(element.at("uuid_component").at("uuid")));
        }

        if(element.contains("transform_component"))
        {
            Leaper::TransformComponent tc;
            Leaper::from_json(element, tc);
            entity.GetComponent<Leaper::TransformComponent>() = tc;
        }

        if(element.contains("sprite_renderer_component"))
        {
            entity.AddComponent<Leaper::SpriteRendererComponent>();
            Leaper::from_json(element, entity.GetComponent<Leaper::SpriteRendererComponent>());
        }

        if(element.contains("rigidbody2d_component"))
        {
            entity.AddComponent<Leaper::Rigidbody2DComponent>();
            Leaper::from_json(element, entity.GetComponent<Leaper::Rigidbody2DComponent>());
        }

        if(element.contains("box_collider2d_component"))
        {
            entity.AddComponent<Leaper::BoxCollider2DComponent>();
            Leaper::from_json(element, entity.GetComponent<Leaper::BoxCollider2DComponent>());
        }
        if(element.contains("lua_script_component"))
        {
            entity.AddComponent<Leaper::LuaScriptComponent>();
            Leaper::from_json(element, entity.GetComponent<Leaper::LuaScriptComponent>());
        }
    }
}

void Leaper::SceneSerializer::Write(const std::string& path)
{
    m_scene->Reg().each([&](auto entity_id)
    {
        Leaper::Entity entity = {entity_id, m_scene.get()};
        WriteEntity(entity);
    });

    std::ofstream file(path);
    file << m_json.dump(4);
}

void Leaper::SceneSerializer::WriteEntity(Leaper::Entity entity)
{
    nlohmann::json j_entity;

    if(entity.HasComponent<Leaper::TagComponent>())
    {
        auto tag = entity.GetComponent<Leaper::TagComponent>();
        Leaper::to_json(j_entity, tag);
    }

    if(entity.HasComponent<Leaper::TransformComponent>())
    {
        auto& trans = entity.GetComponent<Leaper::TransformComponent>();
        Leaper::to_json(j_entity, trans);
    }

    if(entity.HasComponent<Leaper::SpriteRendererComponent>())
    {
        auto& src = entity.GetComponent<Leaper::SpriteRendererComponent>();
        Leaper::to_json(j_entity, src);
    }

    if(entity.HasComponent<Leaper::Rigidbody2DComponent>())
    {
        auto& rb2d = entity.GetComponent<Leaper::Rigidbody2DComponent>();
        Leaper::to_json(j_entity, rb2d);
    }

    if(entity.HasComponent<Leaper::BoxCollider2DComponent>())
    {
        auto& rb2d = entity.GetComponent<Leaper::BoxCollider2DComponent>();
        Leaper::to_json(j_entity, rb2d);
    }

    if(entity.HasComponent<Leaper::LuaScriptComponent>())
    {
        auto& script = entity.GetComponent<Leaper::LuaScriptComponent>();
        Leaper::to_json(j_entity, script);
    }

    if(entity.HasComponent<Leaper::UUIDComponent>())
    {
        auto& uuid = entity.GetComponent<Leaper::UUIDComponent>();
        Leaper::to_json(j_entity, uuid);
    }

    m_json["entities"].push_back(j_entity);
}
