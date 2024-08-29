#pragma once
#include "core/base.h"

#include <string>

namespace Leaper
{
    class Texture
    {
    public:
        virtual uint32_t GetTexture() const = 0;
        virtual std::string& GetPath()      = 0;

        virtual void SetData(void* data) = 0;

        virtual void Bind()                                 = 0;
        virtual void Bind(int slot)                         = 0;
        virtual void UnBind()                               = 0;
        virtual bool operator==(const Texture& other) const = 0;

        static Leaper::Ref<Leaper::Texture> Create(std::string path, bool is_flip = true);
        static Leaper::Ref<Leaper::Texture> Create(uint32_t width, uint32_t height);
    };

}  // namespace Leaper
