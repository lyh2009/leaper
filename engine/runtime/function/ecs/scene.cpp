#include "scene.h"
#include "components.h"
#include "entity.h"
#include "function/render/renderer2d.h"
#include "lppch.h"
#include "systems/audio/audio_system.h"
#include "systems/physics_system/physics2d_system.h"
#include "systems/render_system/animation2d_system.h"
#include "systems/render_system/render_system.h"
#include "systems/script_system/lua_script_system.h"
#include "systems/script_system/navite_script_system.h"

#include <iostream>

Leaper::Entity Leaper::Scene::CreateEntity(const std::string& name)
{
    return CreateEntityWithUUID(name, Leaper::UUID());
}

Leaper::Entity Leaper::Scene::CreateEntityWithUUID(const std::string& name, Leaper::UUID uuid)
{
    Entity entity = { m_registry.create(), this };

    entity.AddComponent<Leaper::UUIDComponent>(uuid);
    entity.AddComponent<Leaper::TagComponent>(name);
    entity.AddComponent<Leaper::TransformComponent>();
    m_entity_map[uuid] = entity;

    return entity;
}

Leaper::Entity Leaper::Scene::CreateNullEntity()
{
    Entity entity = { m_registry.create(), this };
    return entity;
}

Leaper::Entity Leaper::Scene::GetEntityByUUID(Leaper::UUID uuid)
{
    if (m_entity_map.find(uuid) != m_entity_map.end())
        return { m_entity_map.at(uuid), this };

    return {};
}

void Leaper::Scene::DestroyEntity(Leaper::Entity entity)
{
    m_registry.destroy(entity);
}

void Leaper::Scene::OnAttach()
{
    m_systems.emplace_back(Leaper::CreateScope<Leaper::NaviteScriptSystem>(this));
    m_systems.emplace_back(Leaper::CreateScope<Leaper::LuaScriptSystem>(this));
    m_systems.emplace_back(Leaper::CreateScope<Leaper::Physics2D_System>(this));
    m_systems.emplace_back(Leaper::CreateScope<Leaper::RenderSystem>(this));
    m_systems.emplace_back(Leaper::CreateScope<Leaper::Animation2DSystem>(this));
    m_systems.emplace_back(Leaper::CreateScope<Leaper::AudioSystem>(this));

    for (auto& item : m_systems)
        item->OnAttach();
}

void Leaper::Scene::OnUpdate()
{
    for (auto& item : m_systems)
        item->OnUpdate();

    if (is_running)
    {
        for (auto& item : m_systems)
            item->OnGameUpdate();
    }
}

void Leaper::Scene::OnRuntimeStart()
{
    is_running = true;
    for (auto& item : m_systems)
        item->OnGameStart();
}

void Leaper::Scene::OnRuntimeStop()
{
    is_running = false;
    for (auto& item : m_systems)
        item->OnGameStop();
}
