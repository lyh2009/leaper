#include "lppch.h"
#include "lua_components_binder.h"

#include "function/ecs/components.h"

void Leaper::LuaComponentsBinder::BindComponents(sol::state& state)
{
    state.new_usertype<TransformComponent>("Transform", sol::call_constructor, sol::constructors<TransformComponent>(), "position", &TransformComponent::position, "rotation",
                                           &TransformComponent::rotation, "scale", &TransformComponent::scale);
    state.new_usertype<Rigidbody2DComponent>("Rigdbody2D", sol::call_constructor, sol::constructors<Rigidbody2DComponent>(), "velocity", &Rigidbody2DComponent::velocity);
}