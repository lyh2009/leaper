#pragma once
#include <glm/glm.hpp>
#include <random>

namespace Leaper::Math
{
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
    class Random
    {
    public:
        static float FloatRandom(float min, float max);
        static int IntRandom(int min, int max);
    private:
        static std::random_device seed;
        static std::ranlux48 engine;
    };
} // namespace Leaper
