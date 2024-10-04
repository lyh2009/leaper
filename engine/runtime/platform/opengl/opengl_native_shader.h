#pragma once
#include <array>
#include <function/render/shader.h>
#include <glad/glad.h>
namespace Leaper
{
    class OpenGLNativeShader : public Shader
    {
    public:
        OpenGLNativeShader(const std::string& path);
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void GetShaderError(uint32_t shader, Leaper::ShaderType shader_type) override;

        virtual void SetMat4(const std::string value_name, const glm::mat4 value) const override;
        virtual void SetVec4(const std::string value_name, const glm::vec4 value) const override;
        virtual void SetVec3(const std::string value_name, const glm::vec3 value) const override;
        virtual void SetInt(const std::string value_name, const int value) const override;
        virtual void SetFloat(const std::string value_name, const float value) const override;

        virtual uint32_t GetProgram() override
        {
            return m_program;
        }

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    private:
        uint32_t m_program;
        std::string m_name;
    };
}  // namespace Leaper