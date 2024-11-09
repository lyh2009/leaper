#include "lppch.h"
#include "lua_binder.h"


#include "lua_components_binder.h"
#include "lua_entity_binder.h"
#include "lua_globals_binder.h"
#include "lua_maths_binder.h"


void Leaper::LuaBinder::CallBinders(sol::state& state)
{
    state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
    LuaMathsBinder::BindMaths(state);
    LuaComponentsBinder::BindComponents(state);
    LuaEntityBinder::BindEntity(state);
    LuaGlobalsBinder::BindGlobals(state);
}