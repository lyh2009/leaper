#include "function/application/application.h"
#include "glm/fwd.hpp"
#include "render_command.h"

namespace Leaper
{
    Ref<FrameBuffer> RenderCommand::s_main_frame_buffer;
    glm::mat4 RenderCommand::s_light_space_mat;
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

    void RenderCommand::Clear()
    {
        Application::Get().GetRenderAPI()->Clear();
    }
    void RenderCommand::CullFrontFace()
    {
        Application::Get().GetRenderAPI()->CullFrontFace();
    }

    void RenderCommand::EnableCull()
    {
        Application::Get().GetRenderAPI()->EnableCull();
    }
    void RenderCommand::DisableCull()
    {
        Application::Get().GetRenderAPI()->DisableCull();
    }

    void RenderCommand::DepthMask(bool enable)
    {
        Application::Get().GetRenderAPI()->DepthMask(enable);
    }
}  // namespace Leaper