#include <glad/glad.h>
#include "lppch.h"
#include "opengl_buffers.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const void *vertices, size_t size)
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_vbo);
}

void OpenGLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);   
}

void OpenGLVertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void *vertices, size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

//Index Buffer
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
{
    m_count = count;
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_DYNAMIC_DRAW);

}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(sizeof(m_ebo), &m_ebo);
}

void OpenGLIndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void OpenGLIndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLIndexBuffer::SetData(uint32_t* indices, uint32_t count)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ARRAY_BUFFER, count, indices, GL_DYNAMIC_DRAW);
}
