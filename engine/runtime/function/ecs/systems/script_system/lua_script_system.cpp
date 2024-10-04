#include "lppch.h"
#include "lua_script_system.h"


#include "function/ecs/components.h"
#include "function/ecs/entity.h"
#include "function/scripting/lua_binder.h"


#include <filesystem>

void Leaper::LuaScriptSystem::OnAttach() {}
void Leaper::LuaScriptSystem::OnUpdate() {}

void Leaper::LuaScriptSystem::OnGameStart()
{
    LuaBinder::CallBinders(state);
    state.set_function("lpCreateEntity", [&](const std::string& name) -> Entity { return m_scene->CreateEntity(name); });
    m_scene->Reg().view<LuaScriptComponent>().each([&](auto other, LuaScriptComponent& lsc) {
        Entity entity{ other, m_scene };
        auto result = state.safe_script_file(lsc.path, &sol::script_pass_on_error);
        if (result.valid())
        {
            if (result.return_count() == 1 && result[0].is<sol::table>())
            {
                lsc.self         = result[0];
                lsc.self["this"] = &entity;
                LuaCall(lsc, "OnAttach");
            }
        }
        else
        {
            sol::error err = result;
            LP_CORE_LOG_ERROR(err.what());
        }
    });
}

void Leaper::LuaScriptSystem::OnGameUpdate()
{
    m_scene->Reg().view<LuaScriptComponent>().each([&](auto other, LuaScriptComponent& lsc) { LuaCall(lsc, "OnUpdate"); });
}

void Leaper::LuaScriptSystem::OnGameStop()
{
    m_scene->Reg().view<LuaScriptComponent>().each([&](auto other, LuaScriptComponent& lsc) { lsc.self = sol::nil; });
    state = nullptr;
}
