#include "opengl_shader.h"
#include "lppch.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

OpenGLShader::OpenGLShader(std::string vertex_shader_path, std::string fragment_shader_path)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertex_shader_path.c_str());
        fShaderFile.open(fragment_shader_path.c_str());
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    GetShaderError(vertex, Leaper::ShaderType::FragmentShader);
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    GetShaderError(fragment, Leaper::ShaderType::FragmentShader);
    // shader Program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_program);
}

void OpenGLShader::Bind() const
{
    glUseProgram(m_program);
}

void OpenGLShader::UnBind() const
{
    glUseProgram(0);
}

void OpenGLShader::SetVec4(const std::string value_name, const glm::vec4 value) const
{
    uint32_t vec4formLoc = glGetUniformLocation(m_program, value_name.c_str());
    glUniform4f(vec4formLoc, value.r, value.g, value.b, value.a);
}

void OpenGLShader::SetInt(const std::string value_name, const int value) const
{
    uint32_t uint_uniform_location = glGetUniformLocation(m_program, value_name.c_str());
    glUniform1i(uint_uniform_location, value);
}

void OpenGLShader::SetMat4(const std::string value_name, const glm::mat4 value) const
{
    uint32_t mat4formLoc = glGetUniformLocation(m_program, value_name.c_str());
    glUniformMatrix4fv(mat4formLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::GetShaderError(uint32_t shader, Leaper::ShaderType shader_type)
{
    int succes;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
    if (!succes)
    {
        glGetShaderInfoLog(shader, 512, nullptr, log);
        switch (shader_type)
        {
        case Leaper::ShaderType::VertexShader:
            std::cout << "SHADER COMPILE VERTEX SHADER FAILED:" << log << std::endl;
        case Leaper::ShaderType::FragmentShader:
            std::cout << "SHADER COMPILE FRAGMENT SHADER FAILED:" << log << std::endl;
        }
    }
}
