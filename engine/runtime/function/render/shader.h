#pragma once
#include "core/base.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace Leaper
{

    enum class ShaderType
    {
        VertexShader = 0,
        FragmentShader
    };

    class Shader
    {
    public:
        virtual void Bind() const                                                    = 0;
        virtual void UnBind() const                                                  = 0;
        virtual void GetShaderError(uint32_t shader, Leaper::ShaderType shader_type) = 0;

        virtual void SetMat4(const std::string value_name, const glm::mat4 value) const = 0;
        virtual void SetVec4(const std::string value_name, const glm::vec4 value) const = 0;
        virtual void SetVec3(const std::string value_name, const glm::vec3 value) const = 0;
        virtual void SetInt(const std::string value_name, const int value) const        = 0;
        virtual void SetFloat(const std::string value_name, const float value) const    = 0;

        virtual uint32_t GetProgram() = 0;

        static Leaper::Ref<Leaper::Shader> Create(const std::string& name, std::string vertex_shader_path, std::string fragment_shader_path);
        static Leaper::Ref<Leaper::Shader> Create(const std::string& shader_path);
        static Leaper::Ref<Leaper::Shader> CreateNative(const std::string& shader_path);
    };
}  // namespace Leaper
