#pragma once
#include "function/ecs/systems/system.h"

namespace Leaper
{
    class Animation2DSystem : public Leaper::System
    {
    public:
        Animation2DSystem(Leaper::Scene* scene)
            : Leaper::System(scene) {}

        virtual void OnUpdate() override;
    private:
        float m_elapsed = 0.0f;
    };
} // namespace Leaper
