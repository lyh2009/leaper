#include "lua_components_binder.h"
#include "lppch.h"

#include "function/ecs/components.h"
#include "sol/inheritance.hpp"

void Leaper::LuaComponentsBinder::BindComponents(sol::state& state)
{
    state.new_usertype<TransformComponent>("TransformComponent");
    state.new_usertype<Rigidbody2DComponent>("Rigdbody2DComponent");
    state.new_usertype<SoundComponent>("SoundComponent");
}