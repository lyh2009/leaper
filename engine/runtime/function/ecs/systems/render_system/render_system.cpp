#include "lppch.h"
#include "render_system.h"

#include "function/application/application.h"
#include "function/ecs/components.h"
#include "function/ecs/scene.h"
#include "function/render/3d/mesh.h"
#include "function/render/3d/model.h"
#include "function/render/renderer2d.h"
#include "function/render/renderer3d.h"

void Leaper::RenderSystem::OnUpdate()
{

    // sprite
    m_scene->Reg().view<Leaper::SpriteRendererComponent>().each([=](auto other, Leaper::SpriteRendererComponent& src) {
        Leaper::Entity entity = { other, m_scene };
        auto& trans           = entity.GetComponent<TransformComponent>();

        if (entity.HasComponent<Animation2DComponent>()) src.anim = entity.GetComponent<Animation2DComponent>();
        Leaper::Renderer2D::DrawSprite(trans, src.anim, src.m_texture, src.color, (int)other);
    });

    // light
    {
        int i     = 0;
        auto view = m_scene->Reg().view<Leaper::TransformComponent, Leaper::LightComponent>();
        for (auto entity : view)
        {
            auto [trans, light] = view.get<Leaper::TransformComponent, Leaper::LightComponent>(entity);
            light.id            = i;
            Leaper::Renderer2D::DrawLight(trans, light);
            i++;
        }
    }

    // mesh
    m_scene->Reg().view<Leaper::MeshRendererComponment>().each([=](auto other, Leaper::MeshRendererComponment& mrc) {
        Leaper::Entity entity = { other, m_scene };
        auto& trans           = entity.GetComponent<TransformComponent>();
        Renderer3D::DrawModel(mrc.model, trans.GetTransform());
    });
}