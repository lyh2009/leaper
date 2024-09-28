#pragma once

namespace Leaper
{
    class UniformBuffer
    {
    public:
        static Ref<UniformBuffer> Create(size_t size, uint32_t binding);
        virtual void SetData(const void* data, size_t size, uint32_t offset = 0) = 0;
    };
}  // namespace Leaper