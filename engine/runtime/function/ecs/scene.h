#pragma once

#include "function/render/orthographic_camera.h"
#include "core/math/uuid.h"
#include "core/base.h"

#include <glm/glm.hpp>
#include "entt.hpp"

#include <string>
#include <vector>
#include <unordered_map>

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

		void DestroyEntity(Entity entity);
		void OnAttach();
		void OnUpdate(OrthgraphicCamera camera);

		void OnRuntimeStart();
		void OnRuntimeStop();

		inline bool IsRunning() const { return is_running; }

		template<typename... components>
		auto GetAllEntitiesWith()
		{
			return m_registry.view<components...>();
		}

		entt::registry& Reg() { return m_registry; };

	private:
		std::unordered_map<Leaper::UUID, entt::entity> m_entity_map;
		std::vector<Scope<class System>> m_systems;
		entt::registry m_registry;
		bool is_running = false;

		friend class Entity;
	};

	
}