#pragma once
#include "function/ecs/systems/system.h"

namespace Leaper
{
    class NaviteScriptSystem : public Leaper::System
    {
    public:
        NaviteScriptSystem() = delete;
        NaviteScriptSystem(Leaper::Scene* scene)
            : Leaper::System(scene) {}

        virtual void OnUpdate() override;
    };
} // namespace Leaper