#pragma once

#include "core/base.h"
#include "core/math/uuid.h"
#include "function/render/3d/model.h"
#include "function/render/buffers.h"
#include "function/render/game_camera.h"
#include "function/render/orthographic_camera.h"
#include "function/render/texture.h"
#include "function/render/vertex_array.h"

#include "glm/fwd.hpp"
#include "scriptable_entity.h"
#include <sol/sol.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <fmod.hpp>
#include <glm/gtx/quaternion.hpp>

#include <functional>
#include <string>
#include <unordered_map>

namespace Leaper
{
    struct TagComponent
    {
        TagComponent(std::string _tag) : tag(_tag) {}

        inline std::string& Tag()
        {
            return tag;
        }

        std::string tag;
    };

    struct UUIDComponent
    {
        UUIDComponent(Leaper::UUID _uuid) : uuid(_uuid) {}

        Leaper::UUID uuid;
    };

    struct TransformComponent
    {
        glm::vec3 position = { 0, 0, 0 }, rotation = { 0, 0, 0 }, scale = { 1, 1, 1 };

        TransformComponent() : transform(glm::mat4(1.0f)) {};
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& _transform) : transform(_transform) {};

        glm::mat4& GetTransform()
        {
            transform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), scale);

            return transform;
        }

        operator glm::mat4() const
        {
            return transform;
        };

    private:
        glm::mat4 transform = glm::mat4(1.0f);
    };

    struct ParentEntity
    {
        ParentEntity() = default;
        ParentEntity(Entity entity) : parent_entity(entity) {}
        Entity parent_entity;
    };

    struct Animation2DComponent
    {
        int current_frame           = 0;
        int row                     = 1;
        int col                     = 1;
        int speed                   = 24;
        glm::vec2 texture_coords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    };

    struct SpriteRendererComponent
    {
        SpriteRendererComponent() : color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) {};

        SpriteRendererComponent(Leaper::Ref<Leaper::Texture> texture) : m_texture(texture) {};

        SpriteRendererComponent(const SpriteRendererComponent&) = default;

        SpriteRendererComponent(const glm::vec4& _color) : color(_color) {};

        Leaper::Ref<Leaper::Texture> m_texture = nullptr;
        Animation2DComponent anim;
        glm::vec4 color;
        std::string texture_path;
    };

    struct CameraComponent
    {
        CameraComponent(float ratio) : camera(ratio) {};

        GameCamera camera;
    };

    struct NaviteScriptComponent
    {
        Leaper::ScriptableEntity* instance = nullptr;

        std::function<void()> instantiate_function;
        std::function<void()> destory_instance_function;
        std::function<void(Leaper::ScriptableEntity*)> on_create_function;
        std::function<void(Leaper::ScriptableEntity*)> on_update_function;

        template <typename T> void Bind()
        {
            instantiate_function      = [&]() { instance = new T(); };
            destory_instance_function = [&]() {
                delete instance;
                instance = nullptr;
            };
            on_create_function = [](Leaper::ScriptableEntity* script) { ((T*)script)->OnCreate(); };
            on_update_function = [](Leaper::ScriptableEntity* script) { ((T*)script)->OnUpdate(); };
        }
    };

    struct Rigidbody2DComponent
    {
        enum BodyType
        {
            Static = 0,
            Kinematic,
            Dynamic
        };
        BodyType body_type  = Dynamic;
        float gravity_scale = 1.0f;
        glm::vec2 velocity  = glm::vec2(0, 0);
        void* runtime_body  = nullptr;
    };

    struct FixtureUserData
    {
        FixtureUserData() : name("default") {}
        FixtureUserData(std::string user_name) : name(user_name) {}
        std::string name;
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 size = glm::vec2(0.5f, 0.5f);

        float friction    = 0.3f;
        float restitution = 0.0f;
        float density     = 1.0f;

        bool is_trigger = false;

        FixtureUserData* user_data = new FixtureUserData();
    };

    struct CircleCollider2DComponent
    {
        glm::vec2 offset = { 0.0f, 0.0f };

        float radiu       = 1.0f;
        float friction    = 0.3f;
        float restitution = 0.0f;
        float density     = 1.0f;

        bool is_trigger = false;

        FixtureUserData* user_data = new FixtureUserData();
    };

    struct LuaScriptComponent
    {
        LuaScriptComponent() : path("") {}
        LuaScriptComponent(const std::string& script_path) : path(script_path) {}
        sol::table self;
        std::string path;

        std::unordered_map<std::string, int> int_values;
    };

    struct SoundComponent
    {
        SoundComponent()                      = default;
        SoundComponent(const SoundComponent&) = default;
        SoundComponent(const std::string& path)
        {
            this->path = path;
        }

        std::string path = "None";
        bool play;

        FMOD::Sound* sound     = nullptr;
        FMOD::Channel* channel = nullptr;
    };

    struct LightComponent
    {
        LightComponent() = default;
        LightComponent(glm::vec3 color)
        {
            this->color = color;
        }

        glm::vec3 color = glm::vec3(0.999, 0.999, 0.999);
        float intensity = 0.4f;

        int id;
    };

    struct PointLightComponent
    {
        PointLightComponent() = default;
        PointLightComponent(float _constant, float _linear, float _quadratic) : constant(_constant), linear(_linear), quadratic(_quadratic) {}

        float constant  = 1.0f;
        float linear    = 0.09f;
        float quadratic = 0.032f;
    };

    struct CubeRendererComponent
    {
        CubeRendererComponent() : color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) {};

        CubeRendererComponent(Leaper::Ref<Leaper::Texture> texture) : m_texture(texture) {};

        CubeRendererComponent(const CubeRendererComponent&) = default;

        CubeRendererComponent(const glm::vec4& _color) : color(_color) {};

        Leaper::Ref<Leaper::Texture> m_texture = nullptr;

        glm::vec4 color;
        std::string texture_path;
    };

    struct DirectionalLightComponent
    {
        DirectionalLightComponent() = default;
        DirectionalLightComponent(glm::vec3 _color, float _ambient_strength, float _specular_strength)
            : color(_color), ambient_strength(_ambient_strength), specular_strength(_specular_strength)
        {
        }

        glm::vec3 color         = { 1, 1, 1 };
        float ambient_strength  = 0.1f;
        float specular_strength = 0.5f;
    };

    struct MeshRendererComponment
    {
        MeshRendererComponment() = default;
        MeshRendererComponment(const std::string path)
        {
            this->path = path;
            model      = Leaper::Model(path);
        };

        std::string path = "";

        Leaper::Model model;
    };

}  // namespace Leaper