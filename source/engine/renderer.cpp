#include "include/renderer.h"

#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace Engine
{

static constexpr size_t sStrSize = 2048;

class Shader
{
public:
    Shader(const char* vertexPath, int type)
    {
        std::string vertexCode;
        std::ifstream vShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexPath);
            std::stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();

            vShaderFile.close();
            vertexCode = vShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to read shader from file");
        }
        const char* vShaderCode = vertexCode.c_str();
        mID = glCreateShader(type);
        glShaderSource(mID, 1, &vShaderCode, NULL);
        glCompileShader(mID);
        CheckCompileErrors();
    }

    uint32_t GetId() const
    {
        return mID;
    }

private:
    void CheckCompileErrors()
    {
        int success = 0;
        char infoLog[sStrSize];
        glGetShaderiv(mID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(mID, sStrSize, NULL, infoLog);
            
            throw std::runtime_error("Shader compilation error: \n" + std::string(infoLog) + "\n\n");
        }
    }

    uint32_t mID = 0;
};

class Program
{
public:
    Program(std::vector<Shader>&& shaders)
    {
        mID = glCreateProgram();
        for (const auto& shader : shaders)
            glAttachShader(mID, shader.GetId());

        glLinkProgram(mID);
        CheckCompileErrors();

        for (const auto& shader : shaders)
            glDeleteShader(shader.GetId());
    }

    void Use()
    {
        glUseProgram(mID);
    }

    void SetBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(mID, name.c_str()), (int)value);
    }

    void SetInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
    }

    void SetFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
    }

private:
    void CheckCompileErrors()
    {
        int success = 0;
        char infoLog[sStrSize];
        glGetProgramiv(mID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mID, sStrSize, NULL, infoLog);
            throw std::runtime_error("Program linking error: \n" + std::string(infoLog) + "\n\n");
        }
    }

    uint32_t mID = 0;
};

Renderer::Renderer()
{
    std::vector<Shader> shaders;
    shaders.emplace_back("resources/shaders/flag.vs", GL_VERTEX_SHADER);
    shaders.emplace_back("resources/shaders/flag.fs", GL_FRAGMENT_SHADER);

    Program program(std::move(shaders));
}

Renderer::~Renderer() = default;

void Renderer::Render()
{
}

};
