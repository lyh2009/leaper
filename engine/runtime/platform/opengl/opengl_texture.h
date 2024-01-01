#pragma once
#include "function/render/texture.h"

#include <glad/glad.h>

#include <string>

class OpenGLTexture : public Leaper::Texture
{
public:
    OpenGLTexture(uint32_t width, uint32_t height);
    OpenGLTexture(std::string path);

    virtual uint32_t GetTexture() const override { return m_texture; };
    virtual std::string& GetPath() override;

    virtual void SetData(void* data);

    virtual void Bind() override;
    virtual void Bind(int slot) override;
    virtual void UnBind() override;

    virtual bool operator==(const Leaper::Texture& other) const override
	{
		return m_texture == other.GetTexture();
	}
private:
    std::string m_path;

    uint32_t m_width, m_height;
    uint32_t m_texture;
};