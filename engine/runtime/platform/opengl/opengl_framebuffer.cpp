#include "opengl_framebuffer.h"
#include "lppch.h"

#include "function/render/renderer2d.h"
#include "function/application/application.h"

#include <iostream>

OpenGLFrameBuffer::OpenGLFrameBuffer()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_fbo);
	for (int i = 0; i < m_textures.size(); i++)
	{
		glDeleteTextures(1, &m_textures[i]);
	}

}

uint32_t& OpenGLFrameBuffer::GetTexture(int count)
{
	if (count <= m_textures.size())
	{
		return m_textures[count];
	}
	LP_CORE_LOG_ERROR("Failed to load image!");
}

void OpenGLFrameBuffer::RescaleFrameBuffer(float width, float height)
{
	glViewport(0, 0, width, height);
	for (int i = 0; i < m_textures.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, Leaper::Utils::TextureFormatToGL(m_internalformat[i]), width, height, 0, Leaper::Utils::TextureFormatToGL(m_format[i]), GL_UNSIGNED_BYTE, NULL);
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

uint32_t OpenGLFrameBuffer::AttachmentsToGL(Leaper::Attachments attachment)
{
	switch (attachment)
	{
		case Leaper::Attachments::COLOR_ATTACHMENT0:	return GL_COLOR_ATTACHMENT0;
		case Leaper::Attachments::COLOR_ATTACHMENT1:	return GL_COLOR_ATTACHMENT1;
		case Leaper::Attachments::COLOR_ATTACHMENT2:	return GL_COLOR_ATTACHMENT2;
		case Leaper::Attachments::COLOR_ATTACHMENT3:	return GL_COLOR_ATTACHMENT3;
		case Leaper::Attachments::COLOR_ATTACHMENT4:	return GL_COLOR_ATTACHMENT4;
		case Leaper::Attachments::COLOR_ATTACHMENT5:	return GL_COLOR_ATTACHMENT5;
		case Leaper::Attachments::COLOR_ATTACHMENT6:	return GL_COLOR_ATTACHMENT6;
		case Leaper::Attachments::COLOR_ATTACHMENT7:	return GL_COLOR_ATTACHMENT7;
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
	glClearTexImage(m_textures[attachment_index], 0,
	Leaper::Utils::TextureFormatToGL(m_format[attachment_index]), GL_INT, &value);
}

void OpenGLFrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glDrawBuffers(m_attachments.size(), m_attachments.data());
	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); 

	glEnable(GL_PROGRAM_POINT_SIZE);
}

void OpenGLFrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
