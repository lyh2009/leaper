#pragma once
#include "function/ecs/systems/system.h"
#include "function/render/shader.h"

namespace Leaper
{
    class RenderSystem : public Leaper::System
    {
    public:
        RenderSystem() = delete;
        RenderSystem(Leaper::Scene* scene) : Leaper::System(scene) {}

        virtual void OnAttach() override;
        virtual void OnUpdate() override;

    private:
        Ref<Leaper::Shader> shadow_shader;
        glm::vec3 lightPos = { -2.0f, 4.0f, -1.0f };
    };

}  // namespace Leaper
