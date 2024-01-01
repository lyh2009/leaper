#pragma once

#include "function/ecs/systems/system.h"
#include "function/ecs/components.hpp"
#include "core/log.h"

#include <sol/sol.hpp>

#include <string>

namespace Leaper
{
    class LuaScriptSystem : public Leaper::System
    {
    public:
        LuaScriptSystem(Leaper::Scene* scene)
            : Leaper::System(scene) {}
        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual void OnGameStart() override;
        virtual void OnGameUpdate() override;
        virtual void OnGameStop() override;
    
        template<typename ...Args>
        inline static void LuaCall(LuaScriptComponent& lsc, const std::string& name, Args&& ...args)
        {
            sol::function_result result = lsc.self[name].call(std::forward<Args>(args)...);
            if(!result.valid())
            {
                sol::error err = result;
                LP_CORE_LOG_ERROR(err.what());
            }
        }
    private:
        sol::state state;
    };
}