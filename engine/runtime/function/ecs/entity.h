#pragma once
#include "scene.h"

namespace Leaper
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity entity, Scene* scene) : m_scene(scene), m_entity(entity) {}

        template <typename T, typename... Args> T& AddComponent(Args&&... args)
        {
            return m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
        }

        template <typename T> bool HasComponent()
        {
            return m_scene->m_registry.has<T>(m_entity);
        }

        template <typename T> T& GetComponent()
        {
            return m_scene->m_registry.get<T>(m_entity);
        }

        template <typename T> void RemoveComponent()
        {
            LP_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_scene->m_registry.remove<T>(m_entity);
        }

        operator bool() const
        {
            return m_entity != entt::null;
        };
        operator uint32_t() const
        {
            return (uint32_t)m_entity;
        };
        operator entt::entity() const
        {
            return m_entity;
        };

        bool operator==(const Entity& other) const
        {
            return m_entity == other.m_entity && m_scene == other.m_scene;
        };
        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        };

    protected:
        entt::entity m_entity        = entt::null;
        entt::entity m_parent_entity = entt::null;
        Leaper::Scene* m_scene;
    };
}  // namespace Leaper