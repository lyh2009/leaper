#include "lua_entity_binder.h"
#include "lppch.h"

#include "function/ecs/scene.h"
#include "function/ecs/entity.h"
#include "function/ecs/components.hpp"

void Leaper::LuaEntityBinder::BindEntity(sol::state& state)
{
    state.new_usertype<Entity>("lpEntity", sol::call_constructor, sol::constructors<Entity>(),
        "GetTransformComponent", &Entity::GetComponent<TransformComponent>,
        "GetRigdbody2DComponent", &Entity::GetComponent<Rigidbody2DComponent>);
}