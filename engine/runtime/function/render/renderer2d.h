#pragma once
#include "orthographic_camera.h"
#include "vertex_array.h"
#include "game_camera.h"
#include "render_api.h"
#include "buffers.h"
#include "texture.h"
#include "shader.h"

#include "function/ecs/components.hpp"
#include "function/input/input.h"
#include "core/base.h"

#include <glm/glm.hpp>

namespace Leaper
{
    class Renderer2D
    {
    public:
        static void Init(Leaper::Ref<Leaper::RenderAPI> render_api);
        static void BeginScene(Leaper::OrthgraphicCamera camera);
        static void BeginScene(Leaper::OrthgraphicCamera camera, glm::mat4 &trans);
        static void EndScene();
        static void DrawQuad(Leaper::TransformComponent trans, glm::vec4 color, int entity_id = -1);
        static void DrawTexture(Leaper::TransformComponent trans, const Leaper::Ref<Leaper::Texture> &texture, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);
        static void DrawTexture(Leaper::TransformComponent trans, Leaper::Animation2DComponent anim, const Leaper::Ref<Leaper::Texture> &texture, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);
        static void DrawSprite(Leaper::TransformComponent trans, Leaper::Ref<Leaper::Texture> texture, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);
        static void DrawSprite(Leaper::TransformComponent trans, Leaper::Animation2DComponent anim, Leaper::Ref<Leaper::Texture> texture, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);

        static float GetLineWidth() { return m_line_width; };
        static void SetLineWidth(float width);
        static void DrawLine(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color, int entity_id);
        static void DrawRect(Leaper::TransformComponent trans, const glm::vec4 &color, int entity_id);

        static float GetFlushQuadElapsedTime() { return m_flush_quad_elapsed_time; }

        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };

        static void ResetStats();
        static Statistics GetStats();

    private:
        static void StartBatch();
        static void Flush();
        static void NextBatch();

        static Leaper::Ref<Leaper::RenderAPI> m_render_api;
        static float m_line_width;

        static float m_flush_quad_elapsed_time;
    };

} // namespace Leaper
