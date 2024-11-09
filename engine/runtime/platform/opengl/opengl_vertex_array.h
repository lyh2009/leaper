#pragma once
#include "function/render/buffers.h"
#include "function/render/vertex_array.h"

#include <glad/glad.h>

namespace Leaper
{

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual void Bind() override;
        virtual void UnBind() override;
        virtual void AddVertexBuffer(Ref<VertexBuffer> vertex_buffer) override;
        virtual void SetIndexBuffer(Ref<IndexBuffer> index_buffer) override;
        virtual Ref<IndexBuffer> GetIndexBuffer() override
        {
            return m_ebo;
        }
        virtual void DrawArray(uint32_t count) override;

    private:
        uint32_t m_vao;
        uint32_t m_vertex_buffer_index = 0;
        Ref<IndexBuffer> m_ebo;
    };
}  // namespace Leaper