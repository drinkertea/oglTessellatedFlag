#pragma once

#include <cstdint>
#include <string>
#include <chrono>

namespace Engine
{

class Counter
{
public:
    void Update();
    const std::string& GetPresentation() const;

private:
    using TimePoint = std::chrono::system_clock::time_point;

    TimePoint   mPrev        = {};
    uint64_t    mFrameNumber = 0;
    double      mAvgDelta    = 0.0;
    std::string mPresent;
};

}
