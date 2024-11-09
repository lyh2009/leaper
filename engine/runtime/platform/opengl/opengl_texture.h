#pragma once
#include "function/render/texture.h"

#include <glad/glad.h>

#include <string>

namespace Leaper
{

    class OpenGLTexture : public Leaper::Texture
    {
    public:
        OpenGLTexture(uint32_t width, uint32_t height);
        OpenGLTexture(std::vector<std::string> faces_path);
        OpenGLTexture(std::string path, bool is_flip = true);

        virtual uint32_t GetTexture() const override
        {
            return m_texture;
        };
        virtual std::string& GetPath() override;

        virtual void SetData(void* data) override;

        virtual void Bind() override;
        virtual void BindCubeMap() override;
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
}  // namespace Leaper