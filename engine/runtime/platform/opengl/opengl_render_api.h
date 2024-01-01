#pragma once
#include "function/render/render_api.h"
#include "core/base.h"

#include <glad/glad.h>

class OpenGLRenderAPI : public Leaper::RenderAPI
{
public:
    OpenGLRenderAPI();
    ~OpenGLRenderAPI();
    virtual void SetClearColor(glm::vec4 color);
    virtual void Clear();
    virtual void DrawElements(Leaper::Ref<Leaper::VertexArray> vertex_array, uint32_t count) override;
    virtual void DrawLines(const Leaper::Ref<Leaper::VertexArray>& vao, uint32_t count) override;
    virtual void SetLineWidth(const float& width) override;
};