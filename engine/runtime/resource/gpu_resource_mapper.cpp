#include "function/render/texture.h"
#include "gpu_resource_mapper.h"

namespace Leaper
{
    std::unordered_map<std::string, Ref<Texture>> TextureResourceManager::m_map;
    Ref<Texture> TextureResourceManager::m_white_texture;
    std::unordered_map<std::string, Model> ModelResourceManager::m_map;
    Ref<Texture> TextureResourceManager::LoadTexture(const std::string& path, bool flip)
    {
        if (m_map.find(path) == m_map.end())
        {
            auto texture = Texture::Create(path, flip);
            m_map[path]  = texture;
            return texture;
        }

        return m_map[path];
    }

    Model ModelResourceManager::LoadModel(const std::string& path)
    {
        if (m_map.find(path) == m_map.end())
        {
            auto model  = Model(path);
            m_map[path] = model;
            return model;
        }

        return m_map[path];
    }

    Ref<Texture> TextureResourceManager::GetWhiteTexture()
    {
        if (!m_white_texture)
        {
            uint32_t data   = 0xffffffff;
            m_white_texture = Texture::Create(1, 1);
            m_white_texture->SetData(&data);

            return m_white_texture;
        }
        return m_white_texture;
    }
}  // namespace Leaper