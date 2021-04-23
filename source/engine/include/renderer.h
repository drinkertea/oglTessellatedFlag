#pragma once

#include "tessellator.h"

#include <cstdint>
#include <memory>
#include <deque>

namespace Engine
{

class Camera;
class Program;

class Geometry
{
public:
    Geometry(uint32_t depth);
    ~Geometry();

    void Use() const;
    uint32_t GetDepth() const;

    Geometry(const Geometry&) = delete;
    Geometry& operator=(const Geometry&) = delete;

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
    std::unique_ptr<Program>  mProgram;
    std::unique_ptr<Geometry> mGeometry;
};

};
