#pragma  once
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include "vertex_array.h"

namespace Leaper 
{
    class RenderCommand
    {
    public:
        static void SetClearColor(glm::vec4 color);
        static void DrawElements(Leaper::Ref<Leaper::VertexArray> vertex_array, uint32_t count);
        static void DrawLines(const Leaper::Ref<Leaper::VertexArray>& vao, uint32_t count);
        static void SetLineWidth(float width);
    };
}