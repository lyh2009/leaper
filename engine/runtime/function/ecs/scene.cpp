#include "components.h"
#include "entity.h"
#include "function/ecs/components.h"
#include "function/render/renderer2d.h"
#include "lppch.h"
#include "scene.h"
#include "systems/audio/audio_system.h"
#include "systems/physics_system/physics2d_system.h"
#include "systems/render_system/animation2d_system.h"
#include "systems/render_system/render_system.h"
#include "systems/script_system/lua_script_system.h"
#include "systems/script_system/navite_script_system.h"

#include <iostream>

namespace Leaper
{
    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithUUID(name, UUID());
    }

    Entity Scene::CreateEntityWithUUID(const std::string& name, UUID uuid)
    {
        Entity entity = { m_registry.create(), this };

        entity.AddComponent<UUIDComponent>(uuid);
        entity.AddComponent<TagComponent>(name);
        entity.AddComponent<TransformComponent>();
        m_entity_map[uuid] = entity;

        return entity;
    }

    Entity Scene::CreateNullEntity()
    {
        Entity entity = { m_registry.create(), this };
        return entity;
    }

    Entity Scene::GetEntityByUUID(UUID uuid)
    {
        if (m_entity_map.find(uuid) != m_entity_map.end())
            return { m_entity_map.at(uuid), this };

        return {};
    }

    Entity Scene::FindEntityByName(const std::string& name)
    {
        auto view = m_registry.view<TagComponent>();
        for (auto entity : view)
        {
            const TagComponent& tc = view.get<TagComponent>(entity);
            if (name == tc.tag)
                return Entity{ entity, this };
        }
        return {};
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_registry.destroy(entity);
    }

    void Scene::OnAttach()
    {
        m_systems.emplace_back(CreateScope<NaviteScriptSystem>(this));
        m_systems.emplace_back(CreateScope<Physics2D_System>(this));
        m_systems.emplace_back(CreateScope<RenderSystem>(this));
        m_systems.emplace_back(CreateScope<Animation2DSystem>(this));
        m_systems.emplace_back(CreateScope<LuaScriptSystem>(this));
        m_systems.emplace_back(CreateScope<AudioSystem>(this));

        for (auto& item : m_systems) item->OnAttach();
    }

    void Scene::OnUpdate()
    {

        for (auto& item : m_systems) item->OnUpdate();

        if (is_running)
        {
            for (auto& item : m_systems) item->OnGameUpdate();
        }
    }

    void Scene::OnRuntimeStart()
    {
        is_running = true;
        for (auto& item : m_systems) item->OnGameStart();
    }

    void Scene::OnRuntimeStop()
    {
        is_running = false;
        for (auto& item : m_systems) item->OnGameStop();
    }

}  // namespace Leaper
