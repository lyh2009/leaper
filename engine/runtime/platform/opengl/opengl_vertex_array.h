#pragma once
#include "function/render/buffers.h"
#include "function/render/vertex_array.h"

#include <glad/glad.h>

#include <vector>

class OpenGLVertexArray : public Leaper::VertexArray
{
public:
    OpenGLVertexArray();
    virtual void Bind() override;
    virtual void UnBind() override;
    virtual void AddVertexBuffer(Leaper::Ref<Leaper::VertexBuffer> vertex_buffer) override;
    virtual void SetIndexBuffer(Leaper::Ref<Leaper::IndexBuffer> index_buffer) override;
    virtual void DrawArray(uint32_t count) override;

private:
    uint32_t m_vao;
    uint32_t m_vertex_buffer_index = 0;
    Leaper::Ref<Leaper::IndexBuffer> m_ebo;
};
