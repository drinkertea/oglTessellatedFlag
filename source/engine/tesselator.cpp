#include "tessellator.h"

namespace Engine
{

constexpr uint32_t gMaxDepth = 1u << 16u;

Tessellator::Tessellator(uint32_t depth)
{
    Update(depth);
}

void Tessellator::Update(uint32_t depth)
{
    if (depth > gMaxDepth)
        depth = gMaxDepth;

    float delta = 1.0 / static_cast<float>(depth + 1);

    VertexBuffer newVerts;
    IndexBuffer  newInds;

    auto addIndices = [&]()
    {
        uint32_t offset = static_cast<uint32_t>(newVerts.size());
        newInds.push_back(offset + 0);
        newInds.push_back(offset + 1);
        newInds.push_back(offset + 2);
        newInds.push_back(offset + 2);
        newInds.push_back(offset + 3);
        newInds.push_back(offset + 0);
    };

    auto addVertex = [&](float x, float y)
    {
        newVerts.push_back(Vertex{ { x * 1.5, y, 0.0f } });
    };

    // Only vertical split needed for flag
    for (uint32_t x = 0; x < depth + 1; ++x)
    {
        auto xf = static_cast<float>(x) * delta;
        addIndices();
        addVertex(xf, 1.0f);
        addVertex(xf + delta, 1.0f);
        addVertex(xf + delta, 0.0f);
        addVertex(xf, 0.0f);
    }

    mVertices.swap(newVerts);
    mIndices.swap(newInds);
}

const Tessellator::VertexBuffer& Tessellator::GetVertices() const
{
    return mVertices;
}

const Tessellator::IndexBuffer& Tessellator::Getindices() const
{
    return mIndices;
}

};
