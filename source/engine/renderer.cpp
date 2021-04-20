#include "renderer.h"
#include "camera.h"
#include "tessellator.h"

#include <glad/glad.h>
#include <lodepng.h>

#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>

namespace Engine
{

static constexpr size_t sStrSize = 2048;

static const std::array<const char*, 3> gTextures = {
     "resources/textures/algeria.png",
     "resources/textures/merina_people.png",
     "resources/textures/maldives.png",
};

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

    void SetMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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
    shaders.emplace_back("resources/shaders/flag_vertex.glsl", GL_VERTEX_SHADER);
    shaders.emplace_back("resources/shaders/flag_fragment.glsl", GL_FRAGMENT_SHADER);

    mProgram = std::make_unique<Program>(std::move(shaders));
}

Renderer::~Renderer() = default;

void Renderer::Render(const Camera& camera)
{
    const auto& config = camera.CurrentConfig();
    if (!mGeometry || config.depth != mGeometry->GetDepth())
        mGeometry = std::make_unique<Geometry>(config.depth);

    if (config.texture >= gTextures.size())
        throw std::runtime_error("Unexpected texture index.");

    if (!mTexture || gTextures[config.texture] != mTexture->GetPath())
        mTexture = std::make_unique<Texture>(gTextures[config.texture]);

    glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, config.wireframe ? GL_LINE : GL_FILL);

    mProgram->Use();

    mProgram->SetMat4("viewProj", camera.GetViewProjection());
    mProgram->SetFloat("xOffset",   config.xBaseOffset);
    mProgram->SetFloat("time",      config.xTimeOffset);
    mProgram->SetFloat("amplitude", config.amplitude);
    mProgram->SetFloat("waveCount", config.waveCount);

    mTexture->Use();
    mGeometry->Use();
}

Geometry::Geometry(uint32_t depth)
    : mTessellator(depth)
    , mDepth(depth)
{
    glGenVertexArrays(1, &mVertexArray);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);

    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        Tessellator::sVertexSize * mTessellator.GetVertices().size(),
        mTessellator.GetVertices().data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(uint32_t) * mTessellator.Getindices().size(),
        mTessellator.Getindices().data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Geometry::~Geometry()
{
    glDeleteVertexArrays(1, &mVertexArray);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
}

void Geometry::Use() const
{
    glBindVertexArray(mVertexArray);
    glDrawElements(GL_TRIANGLES, mTessellator.Getindices().size(), GL_UNSIGNED_INT, 0);
}

uint32_t Geometry::GetDepth() const
{
    return mDepth;
}

Texture::Texture(const char* path)
    : mPath(path)
{
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    uint8_t* textureData = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
    lodepng_decode24_file(&textureData, &width, &height, path);
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

Texture::~Texture()
{
    glDeleteTextures(1, &mTexture);
}

void Texture::Use() const
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

const char* Texture::GetPath() const
{
    return mPath;
}

};
