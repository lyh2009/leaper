#pragma once
#include <sol/sol.hpp>

namespace Leaper
{
    class LuaGlobalsBinder
    {
    public:
        static void BindGlobals(sol::state& state);
    };

} // namespace Leaper
