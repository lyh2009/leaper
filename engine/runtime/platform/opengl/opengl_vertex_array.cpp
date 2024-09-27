#include "lppch.h"
#include "opengl_vertex_array.h"


#include <glad/glad.h>
#include <glm/glm.hpp>

static GLenum ShaderDataTypeToOpenGLBaseType(Leaper::ShaderDataType type)
{
    switch (type)
    {
    case Leaper::ShaderDataType::Float: return GL_FLOAT;
    case Leaper::ShaderDataType::Float2: return GL_FLOAT;
    case Leaper::ShaderDataType::Float3: return GL_FLOAT;
    case Leaper::ShaderDataType::Float4: return GL_FLOAT;
    case Leaper::ShaderDataType::Mat3: return GL_FLOAT;
    case Leaper::ShaderDataType::Mat4: return GL_FLOAT;
    case Leaper::ShaderDataType::Int: return GL_INT;
    case Leaper::ShaderDataType::Int2: return GL_INT;
    case Leaper::ShaderDataType::Int3: return GL_INT;
    case Leaper::ShaderDataType::Int4: return GL_INT;
    case Leaper::ShaderDataType::Bool: return GL_BOOL;
    }
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void OpenGLVertexArray::Bind()
{
    glBindVertexArray(m_vao);
}

void OpenGLVertexArray::UnBind()
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(Leaper::Ref<Leaper::VertexBuffer> vertex_buffer)
{
    const auto& layout = vertex_buffer->GetLayout();

    for (auto& element : layout)
    {
        switch (element.m_type)
        {
        case Leaper::ShaderDataType::None:
        case Leaper::ShaderDataType::Float:
        case Leaper::ShaderDataType::Float2:
        case Leaper::ShaderDataType::Float3:
        case Leaper::ShaderDataType::Float4: {
            glEnableVertexAttribArray(m_vertex_buffer_index);
            glVertexAttribPointer(m_vertex_buffer_index, element.GetTypeCount(), ShaderDataTypeToOpenGLBaseType(element.m_type), element.m_normalized, layout.GetStride(),
                                  (void*)element.m_offset);
            m_vertex_buffer_index++;
            break;
        }
        case Leaper::ShaderDataType::Int:
        case Leaper::ShaderDataType::Int2:
        case Leaper::ShaderDataType::Int3:
        case Leaper::ShaderDataType::Int4:
        case Leaper::ShaderDataType::Mat3:
        case Leaper::ShaderDataType::Mat4:
        case Leaper::ShaderDataType::Bool: {
            glEnableVertexAttribArray(m_vertex_buffer_index);
            glVertexAttribIPointer(m_vertex_buffer_index, element.GetTypeCount(), ShaderDataTypeToOpenGLBaseType(element.m_type), layout.GetStride(), (void*)element.m_offset);
            m_vertex_buffer_index++;
            break;
        }
        default:
            // LP_ASSERT(false, "Unknown ShaderDataType!");
            break;
            ;
        }
    }
}

void OpenGLVertexArray::SetIndexBuffer(Leaper::Ref<Leaper::IndexBuffer> index_buffer)
{
    glBindVertexArray(m_vao);
    index_buffer->Bind();
    m_ebo = index_buffer;
}

void OpenGLVertexArray::DrawArray(uint32_t count)
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}