#include "window.h"
#include "renderer.h"
#include "camera.h"
#include "config.h"

#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>

int main()
{
    Engine::Window window(1280, 720, false);
    Engine::Config config{};
    Engine::Camera camera(window, config);
    Engine::Renderer renderer;

    auto prev = std::chrono::system_clock::now();
    uint64_t frameNumber = 0;
    double avgDelta = 0.0;
    window.EventLoop([&] {
        camera.Update();
        renderer.Render(camera);

        auto curr = std::chrono::system_clock::now();
        auto delta = std::chrono::duration<double>(curr - prev).count();
        prev = curr;

        if (frameNumber++ == 0)
            avgDelta = delta;
        else
            avgDelta = (avgDelta * (frameNumber - 1) + delta) / frameNumber;

        if (frameNumber % 100 != 0 && delta < 0.01)
            return;

        std::stringstream ss;
        auto fps = static_cast<uint32_t>(std::round(1.0 / delta));
        auto avgFps = static_cast<uint32_t>(std::round(1.0 / avgDelta));
        ss << "FPS: " << std::setw(4) << fps << "; ";
        ss << "AVG: " << std::setw(4) << avgFps << "; ";
        ss << config.ToString();
        window.SetTitle(ss.str());
    });
    return 0;
}
