#pragma once

#include <cstdint>
#include <memory>

namespace Engine
{

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Render();
private:
    class Program;
    std::unique_ptr<Program> mProgram;

    uint32_t mTexture = 0;
    uint32_t mVertexArray = 0;
};

};
