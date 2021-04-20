#include "tessellator.h"

namespace Engine
{



Tessellator::Tessellator(uint32_t depth)
{
    Update(depth);
}

void Tessellator::Update(uint32_t depth)
{
    if (depth > 100)
        depth = 100;

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
        newVerts.push_back(Vertex{ { x, y, 0.0f }, { x, 1.f - y } });
    };

    for (uint32_t x = 0; x < depth + 1; ++x)
    {
        for (uint32_t y = 0; y < depth + 1; ++y)
        {
            auto xf = static_cast<float>(x) * delta;
            auto yf = static_cast<float>(y) * delta;
            addIndices();
            addVertex(xf, yf + delta);
            addVertex(xf + delta, yf + delta);
            addVertex(xf + delta, yf);
            addVertex(xf, yf);
        }
    }
    glm::vec3 translation(-0.5f, -0.5f, 0.0f);
    for (auto& v : newVerts)
        v.position += translation;

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
