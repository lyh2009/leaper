#pragma once

#include "core/base.h"
#include "core/math/uuid.h"
#include "function/render/orthographic_camera.h"


#include "entt.hpp"
#include <glm/glm.hpp>


#include <string>
#include <unordered_map>
#include <vector>


namespace Leaper
{
class Entity;
class Scene
{
public:
    /// @brief Create an entity with TagComponent, UUIDComponent and TransformComponent.
    /// @param name
    /// @return Return the entity
    /// @note UUID are randomly generated
    Entity CreateEntity(const std::string& name);

    /// @brief Create an entity with your own defined UUID
    /// @param name
    /// @param uuid
    /// @return Return the entity
    Entity CreateEntityWithUUID(const std::string& name, Leaper::UUID uuid);

    /// @brief Create an null entity
    /// @return Return the entity
    Entity CreateNullEntity();

    /// @brief Get the entity via UUID
    /// @param uuid
    /// @return Return the entity
    Entity GetEntityByUUID(Leaper::UUID uuid);

    /// @brief Destroy the entity
    /// @param entity
    void DestroyEntity(Entity entity);

    /// @brief Initialize and call the OnAttach functions of each System
    void OnAttach();

    /// @brief Runs the OnUpdate function for each system
    /// @note The OnAttach function must be called first
    void OnUpdate();

    /// @brief Call each system as the function at the start of the game
    /// @note The OnAttach function must be called first
    void OnRuntimeStart();

    /// @brief Call each system as the function at the stop of the game
    /// @note The OnAttach function must be called first
    void OnRuntimeStop();

    /// @brief Return is_running
    inline bool IsRunning() const
    {
        return is_running;
    }

    /// @brief Get all entities that own a component
    /// @tparam ...components
    /// @return Return entities
    template < typename... components > auto GetAllEntitiesWith()
    {
        return m_registry.view< components... >();
    }

    /// @brief Get m_registry
    /// @return Return m_registry
    entt::registry& Reg()
    {
        return m_registry;
    };

private:
    std::unordered_map< Leaper::UUID, entt::entity > m_entity_map;
    std::vector< Scope< class System > >             m_systems;
    entt::registry                                   m_registry;
    bool                                             is_running = false;

    friend class Entity;
};

}  // namespace Leaper