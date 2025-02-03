#include "core/log.h"
#include "lppch.h"
#include "lua_script_system.h"

#include "function/ecs/components.h"
#include "function/ecs/entity.h"
#include "function/scripting/lua_binder.h"
#include "sol/load_result.hpp"

#include <filesystem>

namespace Leaper
{
    void LuaScriptSystem::OnAttach() {}
    void LuaScriptSystem::OnUpdate() {}

    void LuaScriptSystem::OnGameStart()
    {
        LuaBinder::CallBinders(state);
        state.set_function("CreateEntity", [&](const std::string& name) -> Entity { return m_scene->CreateEntity(name); });
        state.set_function("GetEntity", [&](const std::string& name) -> Entity { return m_scene->FindEntityByName(name); });

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
                    const sol::table& globals = lsc.self;
                    for (auto item : globals)
                    {
                        auto key  = item.first;
                        auto type = item.second;
                        if (key.is<std::string>())
                        {
                            if (type.is<int>())
                                lsc.int_values[key.as<std::string>()] = type.as<int>();
                        }
                    }
                }
            }
            else
            {
                sol::error err = result;
                LP_CORE_ERROR(err.what());
            }
        });
    }

    void LuaScriptSystem::OnGameUpdate()
    {
        m_scene->Reg().view<LuaScriptComponent>().each([&](auto other, LuaScriptComponent& lsc) { LuaCall(lsc, "OnUpdate"); });
    }

    void LuaScriptSystem::OnGameStop()
    {
        m_scene->Reg().view<LuaScriptComponent>().each([&](auto other, LuaScriptComponent& lsc) { lsc.self = sol::nil; });
        state = nullptr;
    }

}  // namespace Leaper
