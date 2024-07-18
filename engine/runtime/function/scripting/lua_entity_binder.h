#pragma once
#include "function/ecs/components.h"
#include "function/ecs/entity.h"

#include <sol/sol.hpp>

namespace Leaper
{
class LuaEntityBinder
{
public:
    static void BindEntity(sol::state& state);
};

}  // namespace Leaper