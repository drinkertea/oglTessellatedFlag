#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace Engine
{

class Tessellator
{
public:
    struct Vertex
    {
        glm::vec3 position;
    };
    static constexpr size_t sVertexSize = sizeof(Vertex);
    static_assert(sVertexSize == sizeof(float) * 3);

    using VertexBuffer = std::vector<Vertex>;
    using IndexBuffer  = std::vector<uint32_t>;

    Tessellator(uint32_t depth = 0);
    void Update(uint32_t depth);

    const VertexBuffer& GetVertices() const;
    const IndexBuffer&  Getindices() const;

private:
    VertexBuffer mVertices;
    IndexBuffer  mIndices;
};

};
