#include "function/ecs/components.h"
#include "lppch.h"
#include "scene_serializer.h"

#include <glm/glm.hpp>

#include <fstream>

// vec2

namespace Leaper
{

    void ToJson(nlohmann::json& j, const glm::vec2& t)
    {
        j = { { "x", t.x }, { "y", t.y } };
    }
    void FromJson(const nlohmann::json& j, glm::vec2& t)
    {
        j.at("x").get_to(t.x);
        j.at("y").get_to(t.y);
    }

    // vec3
    void ToJson(nlohmann::json& j, const glm::vec3& t)
    {
        j = { { "x", t.x }, { "y", t.y }, { "z", t.z } };
    }
    void FromJson(const nlohmann::json& j, glm::vec3& t)
    {
        j.at("x").get_to(t.x);
        j.at("y").get_to(t.y);
        j.at("z").get_to(t.z);
    }

    // vec4
    void ToJson(nlohmann::json& j, const glm::vec4& t)
    {
        j = { { "x", t.x }, { "y", t.y }, { "z", t.z }, { "w", t.w } };
    }
    void FromJson(const nlohmann::json& j, glm::vec4& t)
    {
        j.at("x").get_to(t.x);
        j.at("y").get_to(t.y);
        j.at("z").get_to(t.z);
        j.at("w").get_to(t.w);
    }

    // TagComponent
    void ToJson(nlohmann::json& j, const TagComponent& t)
    {
        j = { { "TagComponent", t.tag } };
    }
    void FromJson(const nlohmann::json& j, TagComponent& t)
    {
        j.at("TagComponent").get_to(t.tag);
    }

    // TransformComponent
    void ToJson(nlohmann::json& j, const TransformComponent& t)
    {
        nlohmann::json j_position;
        nlohmann::json j_rotation;
        nlohmann::json j_scale;
        ToJson(j_position, t.position);
        ToJson(j_rotation, t.rotation);
        ToJson(j_scale, t.scale);
        j["TransformComponent"] = { { "position", j_position }, { "rotation", j_rotation }, { "scale", j_scale } };
    }
    void FromJson(const nlohmann::json& j, TransformComponent& t)
    {
        FromJson(j.at("TransformComponent").at("position"), t.position);
        FromJson(j.at("TransformComponent").at("rotation"), t.rotation);
        FromJson(j.at("TransformComponent").at("scale"), t.scale);
    }

    // Camera Component
    void ToJson(nlohmann::json& j, const CameraComponent& t)
    {
        j["CameraComponent"] = { { "ratio", t.camera.GetRatio() } };
    }

    // SpriteRendererComponent
    void ToJson(nlohmann::json& j, const SpriteRendererComponent& t)
    {
        nlohmann::json j_vec4;
        ToJson(j_vec4, t.color);
        if (t.m_texture)
            j["SpriteRendererComponent"] = { { "color", j_vec4 }, { "path", t.m_texture->GetPath() } };
        else
            j["SpriteRendererComponent"] = { { "color", j_vec4 } };
    }
    void FromJson(const nlohmann::json& j, SpriteRendererComponent& t)
    {
        FromJson(j.at("SpriteRendererComponent").at("color"), t.color);
        if (j.at("SpriteRendererComponent").contains("path"))
            t.m_texture = Texture::Create(j.at("SpriteRendererComponent").at("path"));
    }

    void ToJson(nlohmann::json& j, const Rigidbody2DComponent& t)
    {
        j["Rigidbody2DComponent"] = { { "type", t.body_type }, { "gravity_scale", t.gravity_scale } };
    }
    void FromJson(const nlohmann::json& j, Rigidbody2DComponent& t)
    {
        j.at("Rigidbody2DComponent").at("type").get_to(t.body_type);
        j.at("Rigidbody2DComponent").at("gravity_scale").get_to(t.gravity_scale);
    }

    void ToJson(nlohmann::json& j, const BoxCollider2DComponent& t)
    {
        nlohmann::json j_size;
        nlohmann::json j_offset;
        ToJson(j_size, t.size);
        j["BoxCollider2DComponent"] = { { "tag", t.user_data->name },     { "size", j_size },       { "offset", j_offset },        { "friction", t.friction },
                                        { "restitution", t.restitution }, { "density", t.density }, { "is_trigger", t.is_trigger } };
    }
    void FromJson(const nlohmann::json& j, BoxCollider2DComponent& t)
    {
        FromJson(j.at("BoxCollider2DComponent").at("size"), t.size);
        j.at("BoxCollider2DComponent").at("tag").get_to(t.user_data->name);
        j.at("BoxCollider2DComponent").at("friction").get_to(t.friction);
        j.at("BoxCollider2DComponent").at("restitution").get_to(t.restitution);
        j.at("BoxCollider2DComponent").at("density").get_to(t.density);
        j.at("BoxCollider2DComponent").at("is_trigger").get_to(t.is_trigger);
    }

    void ToJson(nlohmann::json& j, const DirectionalLightComponent& t)
    {
        nlohmann::json j_color;
        ToJson(j_color, t.color);
        j["DirectionalLightComponent"] = { { "color", j_color }, { "ambient_strength", t.ambient_strength }, { "specular_strength", t.specular_strength } };
    }

    void FromJson(const nlohmann::json& j, DirectionalLightComponent& t)
    {
        FromJson(j.at("DirectionalLightComponent").at("color"), t.color);
        j.at("DirectionalLightComponent").at("ambient_strength").get_to(t.ambient_strength);
        j.at("DirectionalLightComponent").at("specular_strength").get_to(t.specular_strength);
    }

    void ToJson(nlohmann::json& j, const LuaScriptComponent& t)
    {
        j["LuaScriptComponent"] = { { "script", t.path } };
    }

    void FromJson(const nlohmann::json& j, LuaScriptComponent& t)
    {
        j.at("LuaScriptComponent").at("script").get_to(t.path);
    }

    void ToJson(nlohmann::json& j, const UUIDComponent& t)
    {
        j["UUIDComponent"] = { { "uuid", (uint64_t)t.uuid } };
    }

    void FromJson(const nlohmann::json& j, UUIDComponent& t)
    {
        j.at("UUIDComponent").at("uuid").get_to(t.uuid.GetBase());
    }

    void ToJson(nlohmann::json& j, const SoundComponent& t)
    {
        j["SoundComponent"] = { { "path", t.path.c_str() } };
    }

    void from_jon(const nlohmann::json& j, SoundComponent& t)
    {
        j.at("SoundComponent").at("path").get_to(t.path);
    }

    void ToJson(nlohmann::json& j, const MeshRendererComponment& t)
    {
        j["MeshRendererComponent"] = { { "path", t.path.c_str() } };
    }
    void FromJson(const nlohmann::json& j, MeshRendererComponment& t)
    {
        j.at("MeshRendererComponent").at("path").get_to(t.path);
        t.model = Model(t.path);
    }

    // Scene serializer
    SceneSerializer::SceneSerializer(Ref<Scene> scene) : m_scene(scene) {}

    template <typename T, typename... Args> void ReadComponent(nlohmann::json& j, Entity entity, const std::string& component, Args&&... args)
    {
        if (j.contains(component))
        {
            entity.AddComponent<T>(std::forward<Args>(args)...);
            FromJson(j, entity.GetComponent<T>());
        }
    }

    template <typename T> void WriteComponent(nlohmann::json& j_entity, Entity entity)
    {
        if (entity.HasComponent<T>())
        {
            T& component = entity.GetComponent<T>();
            ToJson(j_entity, component);
        }
    }

    void SceneSerializer::Read(const std::string& path)
    {
        std::ifstream file(path);
        m_json = nlohmann::json::parse(file);

        for (auto& element : m_json)
        {
            Entity entity;
            if (element.contains("TagComponent") && element.contains("UUIDComponent"))
            {
                entity = m_scene->CreateEntityWithUUID(element.at("TagComponent"), UUID(element.at("UUIDComponent").at("uuid")));
            }

            if (element.contains("TransformComponent"))
            {
                TransformComponent tc;
                FromJson(element, tc);
                entity.GetComponent<TransformComponent>() = tc;
            }

            if (element.contains("CameraComponent"))
            {
                float ratio;
                element.at("CameraComponent").at("ratio").get_to(ratio);
                entity.AddComponent<CameraComponent>(ratio);
            }

            ReadComponent<SpriteRendererComponent>(element, entity, "SpriteRendererComponent");
            ReadComponent<Rigidbody2DComponent>(element, entity, "Rigidbody2DComponent");
            ReadComponent<BoxCollider2DComponent>(element, entity, "BoxCollider2DComponent");
            ReadComponent<LuaScriptComponent>(element, entity, "LuaScriptComponent");
            ReadComponent<LuaScriptComponent>(element, entity, "SoundComponent");
            ReadComponent<MeshRendererComponment>(element, entity, "MeshRendererComponent");
            ReadComponent<DirectionalLightComponent>(element, entity, "DirectionalLightComponent");
        }
    }

    void SceneSerializer::Write(const std::string& path)
    {
        m_scene->Reg().each([&](auto entity_id) {
            Entity entity = { entity_id, m_scene.get() };
            WriteEntity(entity);
        });

        std::ofstream file(path);
        file << m_json.dump(4);
    }

    void SceneSerializer::WriteEntity(Entity entity)
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
        WriteComponent<MeshRendererComponment>(j_entity, entity);
        WriteComponent<DirectionalLightComponent>(j_entity, entity);

        m_json.push_back(j_entity);
    }

}  // namespace Leaper