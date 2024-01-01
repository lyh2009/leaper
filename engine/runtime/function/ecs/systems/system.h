#pragma once
#include "function/ecs/scene.h"
#include "core/base.h"

namespace Leaper
{
    class System
    {
    public:
        System() = delete;
        System(Leaper::Scene* scene)
            : m_scene(scene) {}
        virtual void OnImGuiRender() {}
        virtual void OnAttach() {}
        virtual void OnUpdate() {}
        virtual void OnDestory() {}

        virtual void OnGameStart() {}
        virtual void OnGameUpdate() {}
        virtual void OnGameStop() {}
    protected:
        Leaper::Scene* m_scene = nullptr;
    };

} // namespace Leaper
