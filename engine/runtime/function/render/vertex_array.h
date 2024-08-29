#pragma once
#include "buffers.h"
#include "core/base.h"

#include <vector>

namespace Leaper
{
    class VertexArray
    {
    public:
        virtual void Bind()                                                           = 0;
        virtual void UnBind()                                                         = 0;
        virtual void AddVertexBuffer(Leaper::Ref<Leaper::VertexBuffer> vertex_buffer) = 0;
        virtual void SetIndexBuffer(Leaper::Ref<Leaper::IndexBuffer> index_buffer)    = 0;
        virtual void DrawArray(uint32_t count)                                        = 0;

        static Leaper::Ref<Leaper::VertexArray> Create();
    };

}  // namespace Leaper
