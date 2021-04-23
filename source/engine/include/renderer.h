#pragma once

#include <cstdint>
#include <memory>
#include <deque>

namespace Engine
{

class Camera;
class Program;

//! Tesselated rectangle GPU geometry owner.
class Geometry
{
public:
    //! Creates tesselated rectangle vertices.
    Geometry(uint32_t depth);
    ~Geometry();

    //! Bind vertex buffer on pipeline and draw.
    void Use() const;

    //! Returns current depth of tessellation.
    uint32_t GetDepth() const;

    Geometry(const Geometry&) = delete;
    Geometry& operator=(const Geometry&) = delete;

private:
    uint32_t mVertexArray  = 0; //!< GPU handle of vertex heap.
    uint32_t mIndexBuffer  = 0; //!< GPU handle of index bufffer.
    uint32_t mVertexBuffer = 0; //!< GPU handle of vertex buffer.
    uint32_t mDepth        = 0; //!< Level of tesselation.
    uint32_t mIndexCount   = 0; //!< Amount of indices to draw.
};

//! Responsible for scene render.
class Renderer
{
public:
    //! Create program, set up global pipeline states.
    Renderer();
    ~Renderer();

    //! Handle events, draw flag. Should be called after camera update.
    void Render(const Camera& camera);

private:
    std::unique_ptr<Program>  mProgram;  //!< Current program to bind.
    std::unique_ptr<Geometry> mGeometry; //!< Flag geometry.
};

};
