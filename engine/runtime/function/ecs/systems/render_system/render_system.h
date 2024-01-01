#pragma once
#include "function/ecs/systems/system.h"

namespace Leaper
{
    class RenderSystem : public Leaper::System
    {
    public:
        RenderSystem() = delete;
        RenderSystem(Leaper::Scene* scene)
            : Leaper::System(scene) {}

        virtual void OnUpdate() override; 
    };

} // namespace Leaper
