#pragma once

#include <cstdint>
#include <string>
#include <set>

namespace Engine
{

struct Config
{
    float    amplitude   = 0.1f;
    float    waveCount   = 1.7f;
    float    xTimeOffset = 0.0f;
    float    speed       = 0.5f;
    uint32_t depth       = 50;
    uint32_t texture     = 0;
    bool     wireframe   = false;

    float    rAxis[3]    = { -1.0f, 1.0f, 0.5f };
    float    angle       = 30.0f;

    bool OnKeyPressed(int key, const std::set<int>& pressedKeys);
    std::string ToString() const;
};

}
