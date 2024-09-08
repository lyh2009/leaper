#pragma once
#include "3d/model.h"
#include "texture.h"
#include <glm/glm.hpp>

namespace Leaper
{
    class Renderer3D
    {
    public:
        static void Init();
        static void DrawCube(glm::mat4& trans, const Leaper::Ref<Leaper::Texture>& texture, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), int entity_id = -1);
        static void DrawModel(Model model, glm::mat4& trans, int entity_id = -1);

        static void BeginScene(const glm::mat4& projection_view, glm::vec3 position);
        static void BeginScene(const glm::mat4& projection_view, glm::mat4& trans);
        static void EndScene();

    private:
        static void StartBatch();
        static void Flush();
        static void NextBatch();
    };

}  // namespace Leaper