#include "lppch.h"
#include "opengl_render_api.h"

OpenGLRenderAPI::OpenGLRenderAPI() {}

OpenGLRenderAPI::~OpenGLRenderAPI() {}

void OpenGLRenderAPI::Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}
void OpenGLRenderAPI::SetClearColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAPI::DrawElements(Leaper::Ref<Leaper::VertexArray> vertex_array, uint32_t count)
{
    vertex_array->Bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    vertex_array->UnBind();
}

void OpenGLRenderAPI::DrawLines(const Leaper::Ref<Leaper::VertexArray>& vao, uint32_t count)
{
    vao->Bind();
    glDrawArrays(GL_LINES, 0, count);
}

void OpenGLRenderAPI::SetLineWidth(const float& width)
{
    glLineWidth(width);
}