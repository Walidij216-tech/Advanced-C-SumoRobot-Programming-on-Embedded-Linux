#include "Shader.h"
#include <glad/gl.h>
#include "GLError.h"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

Shader::Shader(const std::string& filepath)
    : m_filepath(filepath), m_id(0)
{
    ShaderProgramSource source = parse(filepath);
    m_id = create(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_id));
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_id));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_id, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: Uniform doesn't exist (" << name << ")" << std::endl;
    }
    m_uniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::parse(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        None = -1,
        Vertex = 0,
        Fragment = 1
    };

    std::string line;
    ShaderType type = ShaderType::None;
    std::stringstream stringstreams[2];
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::Vertex;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::Fragment;
            }
        } else {
            stringstreams[(int)type] << line << '\n';
        }
    }

    return { stringstreams[0].str(), stringstreams[1].str() };
}

unsigned int Shader::compile(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    } else {
        std::cout << "Succeded to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
    }

    return id;
}

unsigned int Shader::create(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    /* Vertex shader specifiy where the vertex should be rendered (runs for each vertex) */
    unsigned int vs = compile(GL_VERTEX_SHADER, vertexShader);
    /* Fragment shader runs for each pixel */
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}