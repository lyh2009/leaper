#pragma once
#include "function/render/uniform_buffer.h"
#include <cstdint>

namespace Leaper
{
    class OpenGLUniformBuffer : public UniformBuffer
    {
    public:
        OpenGLUniformBuffer(size_t size, uint32_t binding);
        ~OpenGLUniformBuffer();

        virtual void SetData(const void* data, size_t size, uint32_t offset = 0) override;

    private:
        uint32_t m_ubo;
    };
}  // namespace Leaper