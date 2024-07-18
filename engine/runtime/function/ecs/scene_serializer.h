#pragma once

#include "components.h"
#include "core/base.h"
#include "entity.h"
#include "scene.h"

#include "json.hpp"

#include <string>

namespace Leaper
{
    class SceneSerializer
    {
    public:
        SceneSerializer() = default;
        SceneSerializer(Leaper::Ref<Leaper::Scene> scene);
        void Read(const std::string& path);
        void Write(const std::string& path);

    private:
        void WriteEntity(Leaper::Entity entity);

    private:
        Leaper::Ref<Leaper::Scene> m_scene;
        nlohmann::json m_json;
    };

}  // namespace Leaper
