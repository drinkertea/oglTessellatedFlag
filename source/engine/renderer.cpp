#include "include/renderer.h"

#include <glad/glad.h>
#include <lodepng.h>

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

class Renderer::Program
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

    mProgram = std::make_unique<Program>(std::move(shaders));

    float vertices[] = {
        // positions          // colors           // mTexture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    uint32_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    uint32_t VBO = 0;
    uint32_t EBO = 0;
    glGenVertexArrays(1, &mVertexArray);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    uint8_t* textureData = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
    lodepng_decode24_file(&textureData, &width, &height, "resources/textures/merina_people.png");
    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        throw std::runtime_error("Failed to load texture");
    }
    free(textureData);
}

Renderer::~Renderer() = default;

void Renderer::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, mTexture);

    mProgram->Use();
    glBindVertexArray(mVertexArray);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

};
