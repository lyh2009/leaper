#include "lppch.h"
#include "opengl_native_shader.h"
#include <fstream>

namespace Leaper
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;
        if (type == "geometry")
            return GL_GEOMETRY_SHADER;

        assert(false && "Unknown shader type!");
        return 0;
    }

    OpenGLNativeShader::OpenGLNativeShader(const std::string& path)
    {
        std::string source = ReadFile(path);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        // Extract name from path
        auto lastSlash = path.find_last_of("/\\");
        lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot   = path.rfind('.');
        auto count     = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        m_name         = path.substr(lastSlash, count);
    }

    void OpenGLNativeShader::Bind() const
    {
        glUseProgram(m_program);
    }

    void OpenGLNativeShader::UnBind() const
    {
        glUseProgram(0);
    }

    void OpenGLNativeShader::GetShaderError(uint32_t shader, Leaper::ShaderType shader_type)
    {
        int succes;
        char log[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
        if (!succes)
        {
            glGetShaderInfoLog(shader, 512, nullptr, log);
            switch (shader_type)
            {
            case Leaper::ShaderType::VertexShader: std::cout << "SHADER COMPILE VERTEX SHADER FAILED:" << log << std::endl;
            case Leaper::ShaderType::FragmentShader: std::cout << "SHADER COMPILE FRAGMENT SHADER FAILED:" << log << std::endl;
            }
        }
    }

    void OpenGLNativeShader::SetVec4(const std::string value_name, const glm::vec4 value) const
    {
        uint32_t vec4formLoc = glGetUniformLocation(m_program, value_name.c_str());
        glUniform4f(vec4formLoc, value.r, value.g, value.b, value.a);
    }

    void OpenGLNativeShader::SetVec3(const std::string value_name, const glm::vec3 value) const
    {
        uint32_t vec4formLoc = glGetUniformLocation(m_program, value_name.c_str());
        glUniform3f(vec4formLoc, value.x, value.y, value.z);
    }

    void OpenGLNativeShader::SetInt(const std::string value_name, const int value) const
    {
        uint32_t uint_uniform_location = glGetUniformLocation(m_program, value_name.c_str());
        glUniform1i(uint_uniform_location, value);
    }

    void OpenGLNativeShader::SetFloat(const std::string value_name, const float value) const
    {
        uint32_t uint_uniform_location = glGetUniformLocation(m_program, value_name.c_str());
        glUniform1f(uint_uniform_location, value);
    }

    void OpenGLNativeShader::SetMat4(const std::string value_name, const glm::mat4 value) const
    {
        uint32_t mat4formLoc = glGetUniformLocation(m_program, value_name.c_str());
        glUniformMatrix4fv(mat4formLoc, 1, GL_FALSE, glm::value_ptr(value));
    }

    std::string OpenGLNativeShader::ReadFile(const std::string& path)
    {
        std::string result;
        std::ifstream in(path, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            LP_CORE_LOG_ERROR("Could not open file '{0}'", path);
        }

        return result;
    }
    std::unordered_map<GLenum, std::string> OpenGLNativeShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken  = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos             = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            assert(eol != std::string::npos && "Syntax error");
            size_t begin     = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            assert(ShaderTypeFromString(type) && "Invalid shader type specified");

            size_t nextLinePos                        = source.find_first_not_of("\r\n", eol);
            pos                                       = source.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
        }

        return shaderSources;
    }
    void OpenGLNativeShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program = glCreateProgram();
        std::vector<GLenum> glShaderIDs(shaderSources.size());
        int glShaderIDIndex = 0;
        for (auto& kv : shaderSources)
        {
            GLenum type               = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                LP_CORE_LOG_ERROR("{0}", infoLog.data());
                assert(false && "Shader compilation failure!");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        m_program = program;

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            for (auto id : glShaderIDs) glDeleteShader(id);

            LP_CORE_LOG_ERROR("{0}", infoLog.data());
            assert(false && "NativeOpenGLNativeShader link failure!");
            return;
        }

        for (auto id : glShaderIDs) glDetachShader(program, id);
    }
}  // namespace Leaper