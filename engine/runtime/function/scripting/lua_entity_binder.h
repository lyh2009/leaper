#pragma once
#include "function/ecs/entity.h"
#include "function/ecs/components.hpp"

#include <sol/sol.hpp>

namespace Leaper
{
    class LuaEntityBinder
    {
    public:
        static void BindEntity(sol::state& state);
    };

} // namespace Leaper