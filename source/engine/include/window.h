#pragma once

#include <cstdint>
#include <memory>
#include <functional>

namespace Engine
{

class Window
{
public:
    Window(uint32_t width, uint32_t height);
    ~Window();

    void EventLoop(std::function<void()> callback);

private:
    class WindowImpl;
    std::unique_ptr<WindowImpl> mImpl;
};

};
