#include "lua_components_binder.h"
#include "lppch.h"

#include "function/ecs/components.hpp"

void Leaper::LuaComponentsBinder::BindComponents(sol::state& state)
{
    state.new_usertype<TransformComponent>("lpTransform", sol::call_constructor, sol::constructors<TransformComponent>(),
        "position", &TransformComponent::position,
        "rotation", &TransformComponent::rotation,
        "scale",    &TransformComponent::scale);
    state.new_usertype<Rigidbody2DComponent>("lpRigdbody2D", sol::call_constructor, sol::constructors<Rigidbody2DComponent>(),
        "velocity", &Rigidbody2DComponent::velocity);
}