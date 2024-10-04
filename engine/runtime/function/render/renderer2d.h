#pragma once
#include "buffers.h"
#include "game_camera.h"
#include "orthographic_camera.h"
#include "render_api.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"

#include "core/base.h"
#include "function/ecs/components.h"
#include "function/input/input.h"
#include "perspective_camera_controller.h"

#include <glm/glm.hpp>

namespace Leaper
{
    class Renderer2D
    {
    public:
        static void Init();
        static void BeginScene(const glm::mat4& camera);
        static void BeginScene(const glm::mat4& camera, glm::vec3 camera_position);
        static void BeginScene(const glm::mat4& camera, glm::mat4& trans);
        static void BeginScene(const glm::mat4& camera, glm::mat4& trans, glm::vec3 camera_position);
        static void EndScene();
        static void DrawQuad(Leaper::TransformComponent trans, glm::vec4 color, int entity_id = -1);

        static void DrawTexture(Leaper::TransformComponent trans, const Leaper::Ref<Leaper::Texture>& texture, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                int entity_id = -1);
        static void DrawTexture(Leaper::TransformComponent trans, Leaper::Animation2DComponent anim, const Leaper::Ref<Leaper::Texture>& texture,
                                glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);
        static void DrawSprite(Leaper::TransformComponent trans, Leaper::Ref<Leaper::Texture> texture, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);
        static void DrawSprite(Leaper::TransformComponent trans, Leaper::Animation2DComponent anim, Leaper::Ref<Leaper::Texture> texture,
                               glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);
        static void DrawLight(Leaper::TransformComponent trans, Leaper::LightComponent light);

        static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entity_id = -1);
        static void DrawRect(Leaper::TransformComponent trans, const glm::vec4& color, int entity_id = -1);
        static void DrawCircle(Leaper::TransformComponent trans, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entity_id = -1);
        static void SetLineWidth(float width);
        static void SetAmbientLight(const glm::vec3& color)
        {
            m_ambient_light = color;
        }

        static float GetFlushQuadElapsedTime()
        {
            return m_flush_quad_elapsed_time;
        }
        static float GetLineWidth()
        {
            return m_line_width;
        };

        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const
            {
                return QuadCount * 4;
            }
            uint32_t GetTotalIndexCount() const
            {
                return QuadCount * 6;
            }
        };

        static void ResetStats();
        static Statistics GetStats();

    private:
        static void StartBatch();
        static void Flush();
        static void NextBatch();

        static float m_line_width;
        static glm::vec3 m_ambient_light;
        static float m_flush_quad_elapsed_time;
    };

}  // namespace Leaper
