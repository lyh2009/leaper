#include "animation2d_system.h"
#include "lppch.h"

#include "core/time.h"
#include "function/ecs/components.h"
#include "function/ecs/entity.h"
#include "function/ecs/scene.h"
#include "function/render/renderer2d.h"

void Leaper::Animation2DSystem::OnUpdate()
{
    m_scene->Reg().view<Leaper::Animation2DComponent>().each([=](auto other, Leaper::Animation2DComponent& anim) {
        m_elapsed += Leaper::Time::GetDeltaTime();
        anim.current_frame = int(m_elapsed * anim.speed) % (anim.col * anim.row);
        int current_col    = anim.current_frame % anim.col;
        int current_row    = anim.current_frame / anim.col;

        anim.texture_coords[0] = { 0.0f, 0.0f };
        anim.texture_coords[1] = { 1.0f, 0.0f };
        anim.texture_coords[2] = { 1.0f, 1.0f };
        anim.texture_coords[3] = { 0.0f, 1.0f };
        for (size_t i = 0; i < 4; i++)
        {
            anim.texture_coords[i].x *= 1.0 / anim.col;
            anim.texture_coords[i].y *= 1.0 / anim.row;

            anim.texture_coords[i].x += 1.0 / anim.col * current_col;
            anim.texture_coords[i].y += 1.0 / anim.row * current_row;
        }
    });
}
