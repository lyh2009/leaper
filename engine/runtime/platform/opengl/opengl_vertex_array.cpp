#include "lppch.h"
#include "opengl_vertex_array.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Leaper
{

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float: return GL_FLOAT;
        case ShaderDataType::Float2: return GL_FLOAT;
        case ShaderDataType::Float3: return GL_FLOAT;
        case ShaderDataType::Float4: return GL_FLOAT;
        case ShaderDataType::Mat3: return GL_FLOAT;
        case ShaderDataType::Mat4: return GL_FLOAT;
        case ShaderDataType::Int: return GL_INT;
        case ShaderDataType::Int2: return GL_INT;
        case ShaderDataType::Int3: return GL_INT;
        case ShaderDataType::Int4: return GL_INT;
        case ShaderDataType::Bool: return GL_BOOL;
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

    void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertex_buffer)
    {
        const auto& layout = vertex_buffer->GetLayout();

        for (auto& element : layout)
        {
            switch (element.m_type)
            {
            case ShaderDataType::None:
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4: {
                glEnableVertexAttribArray(m_vertex_buffer_index);
                glVertexAttribPointer(m_vertex_buffer_index, element.GetTypeCount(), ShaderDataTypeToOpenGLBaseType(element.m_type), element.m_normalized, layout.GetStride(),
                                      (void*)element.m_offset);
                m_vertex_buffer_index++;
                break;
            }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            case ShaderDataType::Bool: {
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

    void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> index_buffer)
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
}  // namespace Leaper