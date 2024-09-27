#include "function/application/application.h"
#include "glm/fwd.hpp"
#include "render_command.h"

namespace Leaper
{

    void RenderCommand::SetClearColor(glm::vec4 color)
    {
        Application::Get().GetRenderAPI()->SetClearColor(color);
    }

    void RenderCommand::DrawElements(Ref<VertexArray> vertex_array, uint32_t count)
    {
        Application::Get().GetRenderAPI()->DrawElements(vertex_array, count);
    }

    void RenderCommand::DrawLines(const Ref<VertexArray>& vao, uint32_t count)
    {
        Application::Get().GetRenderAPI()->DrawLines(vao, count);
    }

    void RenderCommand::SetLineWidth(float width)
    {
        Application::Get().GetRenderAPI()->SetLineWidth(width);
    }

}  // namespace Leaper