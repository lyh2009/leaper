#pragma once
#include "function/render/shader.h"

#include <glad/glad.h>

#include <string>

class OpenGLShader : public Leaper::Shader
{
public:
    OpenGLShader(std::string vertex_shader_path, std::string fragment_shader_path);
    ~OpenGLShader();
    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void SetMat4(const std::string value_name, const glm::mat4 value) const override;
    virtual void SetVec4(const std::string value_name, const glm::vec4 value) const override;
    virtual void SetInt(const std::string value_name, const int value) const override;

    virtual void GetShaderError(uint32_t shader, Leaper::ShaderType shader_type) override;

    virtual uint32_t GetProgram() override {return m_program;};

private:
    unsigned int m_program;
};
