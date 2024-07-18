#include "lua_maths_binder.h"
#include "lppch.h"

#include <core/math/math.h>

#include <glm/glm.hpp>

void Leaper::LuaMathsBinder::BindMaths(sol::state& state)
{
    state.new_usertype<glm::vec3>("Vec3", sol::call_constructor, sol::constructors<glm::vec3(float, float, float)>(), "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
    state.new_usertype<glm::vec2>("Vec2", sol::call_constructor, sol::constructors<glm::vec2(float, float)>(), "x", &glm::vec2::x, "y", &glm::vec2::y);

    state.create_named_table("Math");
    state["Math"]["IntRandom"]   = &Math::Random::IntRandom;
    state["Math"]["FloatRandom"] = &Math::Random::FloatRandom;
}