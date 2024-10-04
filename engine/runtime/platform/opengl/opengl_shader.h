#pragma once
#include "function/render/shader.h"

#include <glad/glad.h>
#include <spirv_cross/spirv_glsl.hpp>

#include <string>

class OpenGLShader : public Leaper::Shader
{
public:
    enum class ShaderUniformType
    {
        None = 0,
        Bool,
        Int,
        UInt,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat3,
        Mat4
    };

    class ShaderUniform
    {
    public:
        ShaderUniform() = default;
        ShaderUniform(const std::string& name, ShaderUniformType type, uint32_t size, uint32_t offset);

        const std::string& GetName() const
        {
            return mName;
        }
        ShaderUniformType GetType() const
        {
            return mType;
        }
        uint32_t GetSize() const
        {
            return mSize;
        }
        uint32_t GetOffset() const
        {
            return mOffset;
        }

        // static const std::string& UniformTypeToString(ShaderUniformType type);

    private:
        std::string mName;
        ShaderUniformType mType = ShaderUniformType::None;
        uint32_t mSize          = 0;
        uint32_t mOffset        = 0;
    };

    struct ShaderUniformBuffer
    {
        std::string Name;
        uint32_t Index;
        uint32_t BindingPoint;
        uint32_t Size;
        uint32_t RendererID;
        std::vector<ShaderUniform> Uniforms;
    };

    struct ShaderBuffer
    {
        std::string Name;
        uint32_t Size = 0;
        std::unordered_map<std::string, ShaderUniform> Uniforms;
    };
    OpenGLShader(const std::string& shader_path);
    OpenGLShader(const std::string& name, std::string vertex_shader_path, std::string fragment_shader_path);
    ~OpenGLShader();
    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void SetMat4(const std::string value_name, const glm::mat4 value) const override;
    virtual void SetVec4(const std::string value_name, const glm::vec4 value) const override;
    virtual void SetVec3(const std::string value_name, const glm::vec3 value) const override;
    virtual void SetInt(const std::string value_name, const int value) const override;
    virtual void SetFloat(const std::string value_name, const float value) const override;

    virtual void GetShaderError(uint32_t shader, Leaper::ShaderType shader_type) override;

    virtual uint32_t GetProgram() override
    {
        return m_program;
    };

private:
    std::string ReadFile(const std::string& file_path);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

    void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shader_sources);
    void CompileOrGetOpenGLBinaries();
    void CompileOpenGLBinariesForAmd(GLenum& program, std::array<uint32_t, 2>& glShadersIDs);
    void CreateProgramForAmd();
    void CreateProgram();
    void Reflect(GLenum stage, const std::vector<uint32_t>& shader_data);

private:
    unsigned int m_program;

    std::string m_file_path;
    std::string m_name;

    std::unordered_map<GLenum, std::vector<uint32_t>> m_vkSPIRV;
    std::unordered_map<GLenum, std::vector<uint32_t>> m_glSPIRV;

    std::unordered_map<GLenum, std::string> m_gl_source_code;
};
