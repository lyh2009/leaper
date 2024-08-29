#include "lppch.h"
#include "opengl_texture.h"

#include "core/base.h"
#include "platform/windows/windows_window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
{
    m_width  = width;
    m_height = height;

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

OpenGLTexture::OpenGLTexture(std::string path, bool is_flip)
{
    stbi_set_flip_vertically_on_load(is_flip);
    int width, height, nrChannels;
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum data_format;
        GLenum internal_format;
        if (nrChannels == 1)
        {
            internal_format = GL_RGB;
            data_format     = GL_UNSIGNED_BYTE;
        }
        else if (nrChannels == 3)
        {
            internal_format = GL_RGB8;
            data_format     = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            internal_format = GL_RGBA8;
            data_format     = GL_RGBA;
        }

        m_path = path;
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
        glTextureStorage2D(m_texture, 1, internal_format, width, height);

        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum type = internal_format == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE;
        glTextureSubImage2D(m_texture, 0, 0, 0, width, height, data_format, type, data);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        m_width  = width;
        m_height = height;

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else { std::cout << "import image error" << std::endl; }
}

std::string& OpenGLTexture::GetPath()
{
    return m_path;
}

void OpenGLTexture::SetData(void* data)
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void OpenGLTexture::Bind(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void OpenGLTexture::UnBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}