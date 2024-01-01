#pragma once
#include "function/render/framebuffer.h"
#include "core/base.h"

#include <glad/glad.h>

#include <vector>

class OpenGLFrameBuffer : public Leaper::FrameBuffer
{
public:
    OpenGLFrameBuffer();
	~OpenGLFrameBuffer();

	virtual uint32_t& GetTexture(int count) override;
	virtual void RescaleFrameBuffer(float width, float height) override;

	virtual uint32_t CreateTexture(int width, int height, Leaper::TextureFormat internalformat, Leaper::TextureFormat format, Leaper::Attachments attachment) override;
	virtual uint32_t AttachmentsToGL(Leaper::Attachments attachment) override;


	virtual int ReadPixels(uint32_t attachment, int x, int y) override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
    unsigned int m_fbo;
	std::vector<uint32_t> m_textures;
	std::vector<GLenum> m_attachments;
	std::vector<Leaper::TextureFormat> m_format;
	std::vector<Leaper::TextureFormat> m_internalformat;

	bool* m_has_attachment = nullptr;
};

namespace Leaper::Utils
{
	static GLenum TextureFormatToGL(Leaper::TextureFormat tf)
	{
		switch (tf)
		{
			case Leaper::TextureFormat::RGB:			return GL_RGBA;
			case Leaper::TextureFormat::RGB8:			return GL_RGBA8;
			case Leaper::TextureFormat::R32I:			return GL_R32I;
			case Leaper::TextureFormat::RED_INTEGER:	return GL_RED_INTEGER;
		};

		return 0;
	}

	static int GLTextureFormatToSize(GLenum tf)
	{
		switch (tf)
		{
			case GL_RGBA:			return 4;
			case GL_RGBA8:			return 3;
			case GL_R32I:			return 1;
			case GL_RED_INTEGER:	return 1;
		};

		return 0;
	}
}