#pragma once

#include <cstdint>
#include <string>
#include <set>

namespace Engine
{

//! Global flag sample config.
struct Config
{
    float    amplitude   = 0.2f;                  //!< Waves aplitude.
    float    waveCount   = 0.9f;                  //!< Normalized wave count.
    float    xTimeOffset = 0.0f;                  //!< Current time.
    float    speed       = 0.5f;                  //!< Wave speed.
    uint32_t depth       = 50;                    //!< Tessellation coeficient.
    uint32_t texture     = 0;                     //!< Flag texture index, manages fragment sahders. (currently ignored)
    bool     wireframe   = false;                 //!< Toggles wireframe.
    float    rAxis[3]    = { -1.0f, 1.0f, 0.5f }; //!< Rotation coeficients.
    float    angle       = 30.0f;                 //!< Rotation angle.

    bool OnKeyPressed(int key, const std::set<int>& pressedKeys);
    std::string ToString() const;
};

}
