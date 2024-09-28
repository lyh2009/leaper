#pragma once
#include "function/render/3d/model.h"
#include "function/render/texture.h"
#include "lppch.h"
#include <string>
#include <unordered_map>

namespace Leaper
{
    class TextureResourceManager
    {
    public:
        static Ref<Texture> LoadTexture(const std::string& path, bool flip = true);
        static Ref<Texture> GetWhiteTexture();

    private:
        static std::unordered_map<std::string, Ref<Texture>> m_map;
        static Ref<Texture> m_white_texture;
    };

    class ModelResourceManager
    {
    public:
        static Model LoadModel(const std::string& path);

    private:
        static std::unordered_map<std::string, Model> m_map;
    };
}  // namespace Leaper