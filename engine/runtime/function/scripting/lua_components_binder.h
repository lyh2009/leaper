#pragma once
#include <sol/sol.hpp>

namespace Leaper
{
    class LuaComponentsBinder
    {
    public:
        static void BindComponents(sol::state& state);
    };

} // namespace Leaper