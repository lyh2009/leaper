#include "lppch.h"
#include "lua_entity_binder.h"

#include "function/ecs/components.h"
#include "function/ecs/entity.h"
#include "function/ecs/scene.h"

void Leaper::LuaEntityBinder::BindEntity(sol::state& state)
{
    state.new_usertype<Entity>("Entity", sol::call_constructor, sol::constructors<Entity>(), "GetTransformComponent", &Entity::GetComponent<TransformComponent>,
                               "GetRigdbody2DComponent", &Entity::GetComponent<Rigidbody2DComponent>);
}