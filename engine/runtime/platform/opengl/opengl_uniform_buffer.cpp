#include "lppch.h"
#include "opengl_uniform_buffer.h"
#include "platform/windows/windows_window.h"
namespace Leaper
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(size_t size, uint32_t binding)
    {
        glCreateBuffers(1, &m_ubo);
        glNamedBufferData(m_ubo, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ubo);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_ubo);
    }

    void OpenGLUniformBuffer::SetData(const void* data, size_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_ubo, offset, size, data);
    }

}  // namespace Leaper