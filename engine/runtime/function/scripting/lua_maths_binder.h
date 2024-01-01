#pragma once
#include <sol/sol.hpp>

namespace Leaper
{
    class LuaMathsBinder
    {
    public:
        static void BindMaths(sol::state& state);
    };

} // namespace Leaper
