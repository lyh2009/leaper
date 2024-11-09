#include "core/base.h"
#include "core/log.h"
#include "filesystem"
#include "fstream"
#include "lppch.h"
#include "opengl_shader.h"
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Leaper
{
    namespace Utils
    {

        static GLenum ShaderTypeFromString(const std::string& type)
        {
            if (type == "vertex")
                return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel")
                return GL_FRAGMENT_SHADER;

            LP_ASSERT(false, "Unknown shader type!");
            return 0;
        }

        static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
            case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
            }
            return (shaderc_shader_kind)0;
        }

        static const char* GLShaderStageToString(GLenum stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
            case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
            }
            return nullptr;
        }

        static const char* GetCacheDirectory()
        {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shader/opengl";
        }

        static void CreateCacheDirectoryIfNeeded()
        {
            std::string cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }

        static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER: return ".cached_opengl.vert";
            case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
            }
            return "";
        }

        static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
            case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
            }
            return "";
        }

        static const bool IsAmdGpu()
        {
            const char* vendor = (char*)glGetString(GL_VENDOR);
            return strstr(vendor, "ATI") != nullptr;
        }

        static OpenGLShader::ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
        {
            switch (type.basetype)
            {
            case spirv_cross::SPIRType::Boolean: return OpenGLShader::ShaderUniformType::Bool;
            case spirv_cross::SPIRType::Int: return OpenGLShader::ShaderUniformType::Int;
            case spirv_cross::SPIRType::UInt: return OpenGLShader::ShaderUniformType::UInt;
            case spirv_cross::SPIRType::Float:
                if (type.vecsize == 1)
                    return OpenGLShader::ShaderUniformType::Float;
                if (type.vecsize == 2)
                    return OpenGLShader::ShaderUniformType::Vec2;
                if (type.vecsize == 3)
                    return OpenGLShader::ShaderUniformType::Vec3;
                if (type.vecsize == 4)
                    return OpenGLShader::ShaderUniformType::Vec4;

                if (type.columns == 3)
                    return OpenGLShader::ShaderUniformType::Mat3;
                if (type.columns == 4)
                    return OpenGLShader::ShaderUniformType::Mat4;
                break;
            }
            LP_ASSERT(false, "Unknown type!");
            return OpenGLShader::ShaderUniformType::None;
        }

    }  // namespace Utils
}  // namespace Leaper

namespace Leaper
{

    OpenGLShader::OpenGLShader(const std::string& shader_path) : m_file_path(shader_path)
    {

        Utils::CreateCacheDirectoryIfNeeded();

        std::string source = ReadFile(shader_path);
        auto shaderSources = PreProcess(source);

        {
            CompileOrGetVulkanBinaries(shaderSources);
            if (Utils::IsAmdGpu())
            {
                CreateProgramForAmd();
            }
            else
            {
                CompileOrGetOpenGLBinaries();
                CreateProgram();
            }
        }

        // Extract name from shader_path
        auto lastSlash = shader_path.find_last_of("/\\");
        lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot   = shader_path.rfind('.');
        auto count     = lastDot == std::string::npos ? shader_path.size() - lastSlash : lastDot - lastSlash;
        m_name         = shader_path.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, std::string vertex_shader_path, std::string fragment_shader_path) : m_name(name)
    {
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER]   = vertex_shader_path;
        sources[GL_FRAGMENT_SHADER] = fragment_shader_path;

        CompileOrGetVulkanBinaries(sources);
        if (Utils::IsAmdGpu())
        {
            CreateProgramForAmd();
        }
        else
        {
            CompileOrGetOpenGLBinaries();
            CreateProgram();
        }
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_program);
    }
    static bool VerifyProgramLink(GLenum& program)
    {
        int isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            int maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            LP_CORE_LOG_ERROR("{0}", infoLog.data());
            LP_ASSERT(false, "[OpenGL] Shader link failure!");
            return false;
        }
        return true;
    }

    void OpenGLShader::CreateProgramForAmd()
    {
        GLuint program = glCreateProgram();

        std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();
        std::filesystem::path shaderFilePath = m_file_path;
        std::filesystem::path cachedPath     = cacheDirectory / (shaderFilePath.filename().string() + ".cached_opengl.pgr");
        std::ifstream in(cachedPath, std::ios::ate | std::ios::binary);

        if (in.is_open())
        {
            auto size = in.tellg();
            in.seekg(0);

            auto data       = std::vector<char>(size);
            uint32_t format = 0;
            in.read((char*)&format, sizeof(uint32_t));
            in.read((char*)data.data(), size);
            glProgramBinary(program, format, data.data(), data.size());

            bool linked = VerifyProgramLink(program);

            if (!linked)
                return;
        }
        else
        {
            std::array<uint32_t, 2> glShadersIDs;
            CompileOpenGLBinariesForAmd(program, glShadersIDs);
            glLinkProgram(program);

            bool linked = VerifyProgramLink(program);

            if (linked)
            {
                // Save program data
                GLint formats = 0;
                glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
                LP_ASSERT(formats > 0, "Driver does not support binary format");
                Utils::CreateCacheDirectoryIfNeeded();
                GLint length = 0;
                glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &length);
                auto shaderData = std::vector<char>(length);
                uint32_t format = 0;
                glGetProgramBinary(program, length, nullptr, &format, shaderData.data());
                std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    out.write((char*)&format, sizeof(uint32_t));
                    out.write(shaderData.data(), shaderData.size());
                    out.flush();
                    out.close();
                }
            }

            for (auto& id : glShadersIDs) glDetachShader(program, id);
        }

        m_program = program;
    }

    void OpenGLShader::CompileOpenGLBinariesForAmd(GLenum& program, std::array<uint32_t, 2>& glShadersIDs)
    {
        int glShaderIDIndex = 0;
        for (auto&& [stage, spirv] : m_vkSPIRV)
        {
            spirv_cross::CompilerGLSL glslCompiler(spirv);
            std::string source = glslCompiler.compile();

            uint32_t shader;

            shader = glCreateShader(stage);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            int isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                int maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<char> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                LP_CORE_LOG_ERROR("{0}", infoLog.data());
                LP_ASSERT(false, "[OpenGL] Shader compilation failure!");
                return;
            }
            glAttachShader(program, shader);
            glShadersIDs[glShaderIDIndex++] = shader;
        }
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

    void OpenGLShader::SetVec3(const std::string value_name, const glm::vec3 value) const
    {
        uint32_t vec4formLoc = glGetUniformLocation(m_program, value_name.c_str());
        glUniform3f(vec4formLoc, value.x, value.y, value.z);
    }

    void OpenGLShader::SetInt(const std::string value_name, const int value) const
    {
        uint32_t uint_uniform_location = glGetUniformLocation(m_program, value_name.c_str());
        glUniform1i(uint_uniform_location, value);
    }

    void OpenGLShader::SetFloat(const std::string value_name, const float value) const
    {
        uint32_t uint_uniform_location = glGetUniformLocation(m_program, value_name.c_str());
        glUniform1f(uint_uniform_location, value);
    }

    void OpenGLShader::SetMat4(const std::string value_name, const glm::mat4 value) const
    {
        uint32_t mat4formLoc = glGetUniformLocation(m_program, value_name.c_str());
        glUniformMatrix4fv(mat4formLoc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::GetShaderError(uint32_t shader, ShaderType shader_type)
    {
        int succes;
        char log[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
        if (!succes)
        {
            glGetShaderInfoLog(shader, 512, nullptr, log);
            switch (shader_type)
            {
            case ShaderType::VertexShader: std::cout << "SHADER COMPILE VERTEX SHADER FAILED:" << log << std::endl;
            case ShaderType::FragmentShader: std::cout << "SHADER COMPILE FRAGMENT SHADER FAILED:" << log << std::endl;
            }
        }
    }

    std::string OpenGLShader::ReadFile(const std::string& file_path)
    {
        std::string result;
        std::ifstream in(file_path, std::ios::in | std::ios::binary);  // ifstream closes itself due to RAII
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                LP_CORE_LOG_ERROR("Could not read from file '{0}'", file_path);
            }
        }
        else
        {
            LP_CORE_LOG_ERROR("Could not open file '{0}'", file_path);
        }

        return result;
    }
    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shader_sources;

        const char* typeToken  = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos             = source.find(typeToken, 0);  // Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);  // End of shader type declaration line
            LP_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin     = pos + typeTokenLength + 1;  // Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            LP_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);  // Start of shader code after shader type declaration line
            LP_ASSERT(nextLinePos != std::string::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos);  // Start of next shader type declaration line

            shader_sources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shader_sources;
    }
    void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shader_sources)
    {
        GLuint program = glCreateProgram();

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
        const bool optimize = true;
        if (optimize)
            options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

        auto& shader_data = m_vkSPIRV;
        shader_data.clear();
        for (auto&& [stage, source] : shader_sources)
        {
            std::filesystem::path shaderFilePath = m_file_path;
            std::filesystem::path cachedPath     = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

            std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
            if (in.is_open())
            {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shader_data[stage];
                data.resize(size / sizeof(uint32_t));
                in.read((char*)data.data(), size);
            }
            else
            {
                // LP_CORE_LOG(m_file_path);
                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_file_path.c_str(), options);
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    LP_CORE_LOG_ERROR(module.GetErrorMessage());
                    assert(false);
                }
                if (module.GetCompilationStatus() == shaderc_compilation_status_success)
                {
                    LP_CORE_LOG("Success");
                }

                shader_data[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    auto& data = shader_data[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }

        for (auto&& [stage, data] : shader_data) Reflect(stage, data);
    }
    void OpenGLShader::CompileOrGetOpenGLBinaries()
    {
        auto& shader_data = m_glSPIRV;

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        const bool optimize = false;
        if (optimize)
            options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

        shader_data.clear();
        m_gl_source_code.clear();
        for (auto&& [stage, spirv] : m_vkSPIRV)
        {
            spirv_cross::CompilerGLSL glslCompiler(spirv);

            std::filesystem::path shaderFilePath = m_file_path;
            std::filesystem::path cachedPath     = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

            std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
            if (in.is_open())
            {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shader_data[stage];
                data.resize(size / sizeof(uint32_t));
                in.read((char*)data.data(), size);
            }
            else
            {
                spirv_cross::CompilerGLSL glslCompiler(spirv);
                m_gl_source_code[stage] = glslCompiler.compile();
                auto& source            = m_gl_source_code[stage];

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_file_path.c_str());
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    LP_CORE_LOG_ERROR(module.GetErrorMessage());
                }
                if (module.GetCompilationStatus() == shaderc_compilation_status_success)
                {
                    LP_CORE_LOG("Success");
                }

                shader_data[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    auto& data = shader_data[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }
    }
    void OpenGLShader::CreateProgram()
    {
        GLuint program = glCreateProgram();

        std::vector<GLuint> shaderIDs;
        for (auto&& [stage, spirv] : m_glSPIRV)
        {
            GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
            glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
            glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
            glAttachShader(program, shaderID);
        }

        glLinkProgram(program);

        GLint isLinked;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            LP_CORE_LOG_ERROR("Shader linking failed ({0}):\n{1}", m_file_path, infoLog.data());

            glDeleteProgram(program);

            for (auto id : shaderIDs) glDeleteShader(id);
        }

        for (auto id : shaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_program = program;
    }
    void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shader_data)
    {
        spirv_cross::Compiler compiler(shader_data);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        LP_CORE_LOG_INFO("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_file_path);
        LP_CORE_LOG_INFO("    {0} uniform buffers", resources.uniform_buffers.size());
        LP_CORE_LOG_INFO("    {0} resources", resources.sampled_images.size());

        LP_CORE_LOG_INFO("Uniform buffers:");
        for (const auto& resource : resources.uniform_buffers)
        {
            const auto& bufferType = compiler.get_type(resource.base_type_id);
            uint32_t bufferSize    = compiler.get_declared_struct_size(bufferType);
            uint32_t binding       = compiler.get_decoration(resource.id, spv::DecorationBinding);
            int memberCount        = bufferType.member_types.size();

            LP_CORE_LOG_INFO("  {0}", resource.name);
            LP_CORE_LOG_INFO("    Size = {0}", bufferSize);
            LP_CORE_LOG_INFO("    Binding = {0}", binding);
            LP_CORE_LOG_INFO("    Members = {0}", memberCount);
        }
    }
}  // namespace Leaper