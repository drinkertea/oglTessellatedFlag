#pragma once

#include <cstdint>
#include <memory>
#include <functional>

namespace Engine
{

struct IWindowCallback
{
    virtual void OnKeyEvent(int key, int action) = 0;
    virtual void OnMouseMove(double x, double y) = 0;
    virtual ~IWindowCallback() = default;

    using Ref = std::reference_wrapper<IWindowCallback>;
};

class Window
{
public:
    Window(uint32_t width, uint32_t height, bool vsync);
    ~Window();

    void EventLoop(std::function<void()> callback);
    void AddCallback(IWindowCallback& callback);
    void SetTitle(const std::string& title);

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:
    class WindowImpl;
    std::unique_ptr<WindowImpl> mImpl;
};

};
