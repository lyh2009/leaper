#pragma once
#include "3d/model.h"
#include "core/events/events.h"
#include "function/render/framebuffer.h"
#include "texture.h"
#include <glm/glm.hpp>

namespace Leaper
{
    class Renderer3D
    {
    public:
        static void Init();
        static void DrawCube(glm::mat4& trans, const Leaper::Ref<Leaper::Texture>& texture, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);
        static void DrawModel(Model model, glm::mat4& trans, Ref<Shader> shader, int entity_id = -1);

        static void SetDirectionalLight(glm::vec3 color, float ambient_strength, float specular_strength);

        static void BeginScene(const glm::mat4& ContentBroswerion_view, glm::vec3 position);
        static void SetCamera(const glm::mat4& view, const glm::mat4& ContentBroswerion, glm::vec3 position);
        static void BeginScene(const glm::mat4& ContentBroswerion_view, glm::mat4& trans);
        static void EndScene();

        static void DrawSkyBox(std::vector<std::string> faces);

        static void OnEvent(Event& e);

        static void SetFrameBuffer(Ref<FrameBuffer> frame_buffer)
        {
            s_frame_buffer = frame_buffer;
        }
        static Ref<FrameBuffer> GetFrameBuffer()
        {
            return s_frame_buffer;
        }

    public:
        static Ref<FrameBuffer> s_frame_buffer;

    private:
        static void StartBatch();
        static void Flush();
        static void NextBatch();
    };

}  // namespace Leaper