#include "render_system.h"
#include "lppch.h"

#include "function/ecs/components.hpp"
#include "function/ecs/scene.h"
#include "function/render/renderer2d.h"
#include "function/application/application.h"

void Leaper::RenderSystem::OnUpdate()
{
    m_scene->Reg().view<Leaper::SpriteRendererComponent>().each([=](auto other ,Leaper::SpriteRendererComponent& src)
	{
		Leaper::Entity entity = { other, m_scene };
		auto& trans = entity.GetComponent<TransformComponent>();
		Leaper::Renderer2D::DrawSprite(trans, src.anim, src.m_texture, src.color, (int)other);
	});
}