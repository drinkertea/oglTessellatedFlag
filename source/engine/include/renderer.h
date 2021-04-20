#pragma once

#include "tessellator.h"

#include <cstdint>
#include <memory>

namespace Engine
{

class Camera;

class Geometry
{
public:
    Geometry(uint32_t depth);
    ~Geometry();

    void Use() const;
    uint32_t GetDepth() const;

private:
    uint32_t mVertexArray  = 0;
    uint32_t mIndexBuffer  = 0;
    uint32_t mVertexBuffer = 0;
    uint32_t mDepth        = 0;

    Tessellator mTessellator;
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Render(const Camera& camera);

private:
    class Program;
    std::unique_ptr<Program>  mProgram;
    std::unique_ptr<Geometry> mGeometry;

    uint32_t mTexture{ 0u };
};

};
