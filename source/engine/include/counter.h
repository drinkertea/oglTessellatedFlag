#pragma once

#include <cstdint>
#include <string>
#include <chrono>

namespace Engine
{

//! Simple CPU FPS counter.
class Counter
{
public:
    //! Should be called each frame.
    void Update();

    //! Returns following string: "FPS <cnt>; AVG <cnt>;"
    const std::string& GetPresentation() const;

private:
    using TimePoint = std::chrono::system_clock::time_point;

    TimePoint   mPrev        = {};
    uint64_t    mFrameNumber = 0;
    double      mAvgDelta    = 0.0;
    std::string mPresent;
};

}
