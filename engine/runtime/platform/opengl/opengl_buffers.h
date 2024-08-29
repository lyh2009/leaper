#pragma once
#include "function/render/buffers.h"

#include <glad/glad.h>

class OpenGLVertexBuffer : public Leaper::VertexBuffer
{
public:
    /// @brief Create a Vertex Buffer.
    /// @param size Vertices's size
    OpenGLVertexBuffer(const void *, size_t size);

    /// @brief Create a Vertex Buffer
    OpenGLVertexBuffer(size_t size);

    ~OpenGLVertexBuffer();

    /// @brief Bind Vertex Buffer.
    virtual void Bind() override;

    /// @brief UnBind Vertex Buffer
    virtual void UnBind() override;

    /// @brief Set Vertex Buffer's Data
    virtual void SetData(const void *, size_t size) override;


    /// @brief Set Layout
    virtual void SetLayout(const Leaper::BufferLayout &layout) override { m_layout = layout; }

    /// @brief Return Leaper::OpenGLVertexBuffer::m_layput
    /// @return m_layput
    virtual const Leaper::BufferLayout &GetLayout() const override { return m_layout; }

private:
    Leaper::BufferLayout m_layout;
    uint32_t m_vbo;
};

// Index Buffer
class OpenGLIndexBuffer : public Leaper::IndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t *indices, uint32_t size);
    OpenGLIndexBuffer(uint32_t size);
    ~OpenGLIndexBuffer();
    virtual void Bind() override;
    virtual void UnBind() override;
    virtual uint32_t GetCount() override { return m_count; }
    virtual void SetData(uint32_t *indices, uint32_t count) override;

private:
    uint32_t m_count;
    uint32_t m_ebo;
};