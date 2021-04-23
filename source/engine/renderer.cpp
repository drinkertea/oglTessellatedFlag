#include "renderer.h"
#include "camera.h"
#include "tessellator.h"

#include <glad/glad.h>
#include <lodepng.h>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>

namespace Engine
{

static constexpr size_t sStrSize = 2048;

//! Simple GL shader loader
class Shader
{
public:
    Shader(const char* path, int type)
    {
        std::string vertexCode;
        std::ifstream vShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(path);
            std::stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();

            vShaderFile.close();
            vertexCode = vShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            throw std::runtime_error("Unable to read shader from file, path: " + std::string(path));
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

//! Simgple GL program wrappper
class Program
{
public:
    //! Shaders will be deleted after linkage
    Program(std::vector<Shader>&& shaders)
    {
        mID = glCreateProgram();
        for (const auto& shader : shaders)
            glAttachShader(mID, shader.GetId());

        glLinkProgram(mID);

        std::shared_ptr<void> deleteGuard(nullptr, [&](void*){
            for (const auto& shader : shaders)
                glDeleteShader(shader.GetId()); 
        });
        CheckCompileErrors();
    }

    void Use()
    {
        glUseProgram(mID);
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

Geometry::Geometry(uint32_t depth)
    : mDepth(depth)
{
    Tessellator tessellator(depth);
    glGenVertexArrays(1, &mVertexArray);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);

    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        Tessellator::sVertexSize * tessellator.GetVertices().size(),
        tessellator.GetVertices().data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(uint32_t) * tessellator.Getindices().size(),
        tessellator.Getindices().data(),
        GL_STATIC_DRAW
    );

    mIndexCount = static_cast<uint32_t>(tessellator.Getindices().size());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Tessellator::sVertexSize, (void*)0);
    glEnableVertexAttribArray(0);
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
    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
}

uint32_t Geometry::GetDepth() const
{
    return mDepth;
}

Renderer::Renderer()
{
    std::vector<Shader> shaders;
    shaders.emplace_back("resources/shaders/flag_vertex.glsl", GL_VERTEX_SHADER);
    shaders.emplace_back("resources/shaders/flag_fragment.glsl", GL_FRAGMENT_SHADER);

    mProgram = std::make_unique<Program>(std::move(shaders));

    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() = default;

void Renderer::Render(const Camera& camera)
{
    const auto& config = camera.CurrentConfig();

    // Update geometry if config changed.
    if (!mGeometry || config.depth != mGeometry->GetDepth())
    {
        mGeometry = std::make_unique<Geometry>(config.depth);
    }

    glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Toggle wireframe
    glPolygonMode(GL_FRONT_AND_BACK, config.wireframe ? GL_LINE : GL_FILL);

    mProgram->Use();

    // Apply model transformations.
    glm::mat4 model = glm::identity<glm::mat4>();
    glm::vec3 axis(config.rAxis[0], config.rAxis[1], config.rAxis[2]);
    if (axis != glm::vec3(0.0f, 0.0f, 0.0f))
        model = glm::rotate(model, glm::radians(config.angle), axis);
    model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));

    // Uniforms filling.
    mProgram->SetMat4( "viewProj",  camera.GetViewProjection());
    mProgram->SetMat4( "model",     model);
    mProgram->SetFloat("time",      config.xTimeOffset);
    mProgram->SetFloat("amplitude", config.amplitude);
    mProgram->SetFloat("waveCount", config.waveCount);

    // Draw
    mGeometry->Use();
}

};
