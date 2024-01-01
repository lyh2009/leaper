#pragma once
#include "core/base.h"

#include <stdint.h>
#include <vector>

namespace Leaper
{
    enum class TextureFormat
    {
        RGB = 0,
        RGB8,
        R32I,
        RED_INTEGER
    };

    enum class Attachments
    {
        NONE = 0,
        COLOR_ATTACHMENT0,
        COLOR_ATTACHMENT1,
        COLOR_ATTACHMENT2,
        COLOR_ATTACHMENT3,
        COLOR_ATTACHMENT4,
        COLOR_ATTACHMENT5,
        COLOR_ATTACHMENT6,
        COLOR_ATTACHMENT7,
    };

    class FrameBuffer
    {
    public:
        virtual uint32_t& GetTexture(int count) = 0;
        virtual void RescaleFrameBuffer(float width, float height) = 0;
        virtual int ReadPixels(uint32_t attachment, int x, int y) = 0;
        virtual uint32_t CreateTexture(int width, int height, Leaper::TextureFormat internalformat, Leaper::TextureFormat format, Leaper::Attachments attachment) = 0;
        virtual uint32_t AttachmentsToGL(Leaper::Attachments attachment) = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Leaper::Ref<Leaper::FrameBuffer> Create();
    };

} // namespace Leaper
