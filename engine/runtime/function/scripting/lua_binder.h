#pragma once
#include <sol/sol.hpp>

namespace Leaper
{
    class LuaBinder
    {
    public:
        static void CallBinders(sol::state& state);
    };
}