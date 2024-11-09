#pragma once
#include "core/base.h"
#include "function/render/framebuffer.h"

#include <glad/glad.h>

#include <vector>

namespace Leaper
{

    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer();
        ~OpenGLFrameBuffer();

        virtual uint32_t& GetTexture(int count) override;
        virtual uint32_t& GetDepthTexture() override;
        virtual void RescaleFrameBuffer(float width, float height) override;

        virtual uint32_t CreateTexture(int width, int height, TextureFormat internalformat, TextureFormat format, Attachments attachment) override;
        virtual uint32_t CreateDepthTexture(int width, int height) override;
        virtual uint32_t CreateDepthMap(int width, int height) override;

        virtual uint32_t AttachmentsToGL(Attachments attachment) override;

        virtual void BindDepthTexture(uint32_t active) override;
        virtual void UnBindDepthTexture() override;
        virtual void BindDepthFBO() override;
        virtual void UnBindDepthFBO() override;

        virtual int ReadPixels(uint32_t attachment, int x, int y) override;
        virtual void ClearAttachment(uint32_t attachment_index, int value) override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        unsigned int m_fbo;
        unsigned int m_depth_texture;
        unsigned int m_depth_map;
        unsigned int m_depth_fbo;
        unsigned int pbo;
        std::vector<uint32_t> m_textures;
        std::vector<GLenum> m_attachments;
        std::vector<TextureFormat> m_format;
        std::vector<TextureFormat> m_internalformat;

        bool* m_has_attachment = nullptr;

        float m_width, m_height;
    };

}  // namespace Leaper