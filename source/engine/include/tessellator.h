#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace Engine
{

//! Helper class to own CPU-side vertex data.
class Tessellator
{
public:
    //! Per-vertex data.
    struct Vertex
    {
        glm::vec3 position; //!< Vertex position.
    };
    static constexpr size_t sVertexSize = sizeof(Vertex);
    static_assert(sVertexSize == sizeof(float) * 3);

    using VertexBuffer = std::vector<Vertex>;
    using IndexBuffer  = std::vector<uint32_t>;

    //! Calls Update(depth)
    Tessellator(uint32_t depth = 0);

    //! Creates tesseleted flag rectangle with dim=(1.5, 1.0).
    //! @param[in] depth    Count of vertical splits.
    void Update(uint32_t depth);

    //! Returns CPU vertex buffer.
    const VertexBuffer& GetVertices() const;
    //! Returns CPU index buffer.
    const IndexBuffer&  Getindices() const;

private:
    VertexBuffer mVertices; //!< CPU vertex buffer.
    IndexBuffer  mIndices;  //!< CPU index buffer.
};

};
