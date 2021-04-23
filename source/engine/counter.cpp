#include "counter.h"

#include <iostream>
#include <iomanip>

namespace Engine
{

void Counter::Update()
{
    auto curr = std::chrono::system_clock::now();
    auto delta = std::chrono::duration<double>(curr - mPrev).count();
    mPrev = curr;

    ++mFrameNumber;
    // Use first two frames for initialization
    if (mFrameNumber <= 2)
    {
        if (mFrameNumber == 2)
        {
            mAvgDelta = delta;
        }
        return;
    }
    mAvgDelta = (mAvgDelta * (mFrameNumber - 1) + delta) / mFrameNumber;

    if (mFrameNumber % 100 != 0 && delta < 0.01)
    {
        return;
    }

    std::stringstream ss;
    auto fps = static_cast<uint32_t>(std::round(1.0 / delta));
    auto avgFps = static_cast<uint32_t>(std::round(1.0 / mAvgDelta));
    ss << "FPS: " << std::setw(4) << fps << "; ";
    ss << "AVG: " << std::setw(4) << avgFps << "; ";
    mPresent = ss.str();
}

const std::string& Counter::GetPresentation() const
{
    return mPresent;
}

}
