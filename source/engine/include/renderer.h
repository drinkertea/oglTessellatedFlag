#pragma once

#include "tessellator.h"

#include <cstdint>
#include <memory>

namespace Engine
{

class Camera;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Render(const Camera& camera);

private:
    class Program;
    std::unique_ptr<Program> mProgram;

    uint32_t mTexture = 0;
    uint32_t mVertexArray = 0;

    Tessellator mTessellator;
};

};
