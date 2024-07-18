#include "lua_components_binder.h"
#include "lppch.h"

#include "function/ecs/components.h"

void Leaper::LuaComponentsBinder::BindComponents(sol::state& state)
{
    state.new_usertype<TransformComponent>("TransformComponent", sol::call_constructor, sol::constructors<TransformComponent>(), "position", &TransformComponent::position,
                                           "rotation", &TransformComponent::rotation, "scale", &TransformComponent::scale);
    state.new_usertype<Rigidbody2DComponent>("Rigdbody2DComponent", sol::call_constructor, sol::constructors<Rigidbody2DComponent>(), "velocity", &Rigidbody2DComponent::velocity);
    state.new_usertype<SoundComponent>("SoundComponent", sol::call_constructor, sol::constructors<SoundComponent>(), "path", &SoundComponent::path);
}