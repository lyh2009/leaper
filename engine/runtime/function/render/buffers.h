#pragma once
#include "core/base.h"

#include <vector>
#include <string>

namespace Leaper
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32_t GetShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:     return 4;
            case ShaderDataType::Float2:    return 4 * 2;
            case ShaderDataType::Float3:    return 4 * 3;
            case ShaderDataType::Float4:    return 4 * 4;
            case ShaderDataType::Int:       return 4;
            case ShaderDataType::Int2:      return 4 * 2;
            case ShaderDataType::Int3:      return 4 * 3;
            case ShaderDataType::Int4:      return 4 * 4;
            case ShaderDataType::Mat3:      return 4 * 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4 * 4;
            case ShaderDataType::Bool:      return 1;
        }

        return 0;
    }

    struct BufferElement
    {
        std::string m_name;
        size_t m_size;
        size_t m_offset;
        ShaderDataType m_type;
        bool m_normalized;

        BufferElement(ShaderDataType type, std::string name, bool normalized = false)
            : m_type(type), m_name(name), m_size(GetShaderDataTypeSize(type)), m_normalized(normalized), m_offset(0)
        {
        }

        uint32_t GetTypeCount() const
        {
            switch (m_type)
            {
                case ShaderDataType::Float:   return 1;
                case ShaderDataType::Float2:  return 2;
                case ShaderDataType::Float3:  return 3;
                case ShaderDataType::Float4:  return 4;
                case ShaderDataType::Mat3:    return 3;
                case ShaderDataType::Mat4:    return 4;
                case ShaderDataType::Int:     return 1;
                case ShaderDataType::Int2:    return 2;
                case ShaderDataType::Int3:    return 3;
                case ShaderDataType::Int4:    return 4;
                case ShaderDataType::Bool:    return 1;
            }

            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement> &list)
            : m_buffer_element(list)
        {
            CalculateStride();
        }

        std::vector<BufferElement> GetBufferElement() { return m_buffer_element; }
        std::vector<BufferElement>::iterator begin() { return m_buffer_element.begin(); }
        std::vector<BufferElement>::iterator end() { return m_buffer_element.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_buffer_element.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_buffer_element.end(); }
        uint32_t GetStride() const { return m_stride; }

    private:
        void CalculateStride()
        {
            size_t offset = 0;
            m_stride = 0;
            for (auto &element : m_buffer_element)
            {
                element.m_offset = offset;
                offset += element.m_size;
                m_stride += element.m_size;
            }
        }

    private:
        std::vector<BufferElement> m_buffer_element;
        uint32_t m_stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void SetData(const void * vertices, size_t size) = 0;
        virtual const Leaper::BufferLayout &GetLayout() const = 0;
        virtual void SetLayout(const Leaper::BufferLayout& layout)  = 0;

        static Leaper::Ref<Leaper::VertexBuffer> Create(size_t size);
        static Leaper::Ref<Leaper::VertexBuffer> Create(const void * vertices, size_t size);
    };

    class IndexBuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void SetData(uint32_t* indices, uint32_t count) = 0;
        virtual uint32_t GetCount() = 0;
        static Leaper::Ref<Leaper::IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };

} // namespace Leaper
