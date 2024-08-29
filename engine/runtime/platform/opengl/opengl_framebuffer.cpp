#include "function/render/framebuffer.h"
#include "lppch.h"
#include "opengl_framebuffer.h"

#include "function/application/application.h"
#include "function/render/renderer2d.h"

#include <cstdint>
#include <iostream>

OpenGLFrameBuffer::OpenGLFrameBuffer()
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
    for (int i = 0; i < m_textures.size(); i++) { glDeleteTextures(1, &m_textures[i]); }
}

uint32_t& OpenGLFrameBuffer::GetTexture(int count)
{
    if (count <= m_textures.size()) { return m_textures[count]; }
    LP_CORE_LOG_ERROR("Failed to load image!");
}

void OpenGLFrameBuffer::RescaleFrameBuffer(float width, float height)
{
    glViewport(0, 0, width, height);
    for (int i = 0; i < m_textures.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, Leaper::Utils::TextureFormatToGL(m_internalformat[i]), width, height, 0, Leaper::Utils::TextureFormatToGL(m_format[i]), GL_UNSIGNED_BYTE,
                     NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachments[i], GL_TEXTURE_2D, m_textures[i], 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

uint32_t OpenGLFrameBuffer::CreateTexture(int width, int height, Leaper::TextureFormat internalformat, Leaper::TextureFormat format, Leaper::Attachments attachment)
{
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, Leaper::Utils::TextureFormatToGL(internalformat), width, height, 0, Leaper::Utils::TextureFormatToGL(format), GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentsToGL(attachment), GL_TEXTURE_2D, texture, 0);
    m_internalformat.emplace_back(internalformat);
    m_format.emplace_back(format);
    m_textures.emplace_back(texture);
    m_attachments.emplace_back(AttachmentsToGL(attachment));
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

uint32_t OpenGLFrameBuffer::CreateDepthTexture(int width, int height)
{
    uint32_t textureColorbuffer = m_depth_buffer;
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);                    // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);  // now actually attach it

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureColorbuffer;
}

uint32_t OpenGLFrameBuffer::AttachmentsToGL(Leaper::Attachments attachment)
{
    switch (attachment)
    {
    case Leaper::Attachments::COLOR_ATTACHMENT0: return GL_COLOR_ATTACHMENT0;
    case Leaper::Attachments::COLOR_ATTACHMENT1: return GL_COLOR_ATTACHMENT1;
    case Leaper::Attachments::COLOR_ATTACHMENT2: return GL_COLOR_ATTACHMENT2;
    case Leaper::Attachments::COLOR_ATTACHMENT3: return GL_COLOR_ATTACHMENT3;
    case Leaper::Attachments::COLOR_ATTACHMENT4: return GL_COLOR_ATTACHMENT4;
    case Leaper::Attachments::COLOR_ATTACHMENT5: return GL_COLOR_ATTACHMENT5;
    case Leaper::Attachments::COLOR_ATTACHMENT6: return GL_COLOR_ATTACHMENT6;
    case Leaper::Attachments::COLOR_ATTACHMENT7: return GL_COLOR_ATTACHMENT7;
    case Leaper::Attachments::DEPTH_STENCIL_ATTACHMENT: return GL_DEPTH_STENCIL_ATTACHMENT;
    }

    return 0;
}

int OpenGLFrameBuffer::ReadPixels(uint32_t attachment, int x, int y)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
    int pixels;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixels);
    return pixels;
}

void OpenGLFrameBuffer::ClearAttachment(uint32_t attachment_index, int value)
{
    glClearTexImage(m_textures[attachment_index], 0, Leaper::Utils::TextureFormatToGL(m_format[attachment_index]), GL_INT, &value);
}

void OpenGLFrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawBuffers(m_attachments.size(), m_attachments.data());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLFrameBuffer::Unbind() const
{
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
