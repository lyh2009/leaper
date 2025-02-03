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
        Entity CreateEntity(const std::string& name);

        Entity CreateEntityWithUUID(const std::string& name, Leaper::UUID uuid);

        Entity CreateNullEntity();

        Entity GetEntityByUUID(Leaper::UUID uuid);
        Entity FindEntityByName(const std::string& name);

        void DestroyEntity(Entity entity);

        void OnAttach();

        void OnUpdate();

        void OnRuntimeStart();

        void OnRuntimeStop();

        inline bool IsRunning() const
        {
            return is_running;
        }

        template <typename... components> auto GetAllEntitiesWith()
        {
            return m_registry.view<components...>();
        }

        /// @brief Get m_registry
        /// @return Return m_registry
        entt::registry& Reg()
        {
            return m_registry;
        };

    private:
        std::unordered_map<Leaper::UUID, entt::entity> m_entity_map;
        std::vector<Scope<class System>> m_systems;
        entt::registry m_registry;
        bool is_running = false;

        friend class Entity;
    };

}  // namespace Leaper