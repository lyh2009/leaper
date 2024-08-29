#include "render_command.h"
#include "function/application/application.h"
#include "glm/fwd.hpp"

void Leaper::RenderCommand::SetClearColor(glm::vec4 color)
{
    Leaper::Application::Get().GetRenderAPI()->SetClearColor(color);
}

void Leaper::RenderCommand::DrawElements(Leaper::Ref<Leaper::VertexArray> vertex_array, uint32_t count)
{
    Leaper::Application::Get().GetRenderAPI()->DrawElements(vertex_array, count);
}

void Leaper::RenderCommand::DrawLines(const Leaper::Ref<Leaper::VertexArray>& vao, uint32_t count)
{
    Leaper::Application::Get().GetRenderAPI()->DrawLines(vao, count);
}

void Leaper::RenderCommand::SetLineWidth(float width)
{
    Leaper::Application::Get().GetRenderAPI()->SetLineWidth(width);
}