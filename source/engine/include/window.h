#pragma once

#include <cstdint>
#include <memory>
#include <functional>

namespace Engine
{

//! Window events callback
struct IWindowCallback
{
    //! Keyboard event callback.
    //! @param[in] key      GLFW keyboard key.
    //! @param[in] action   GLFW key action.
    virtual void OnKeyEvent(int key, int action) = 0;

    //! Mouse event callback, xy - mouse position.
    virtual void OnMouseMove(double x, double y) = 0;

    virtual ~IWindowCallback() = default;

    using Ref = std::reference_wrapper<IWindowCallback>;
};

//! Simple GLFW window wrapper.
class Window
{
public:
    //! Creates window instance. Should be only one per app.
    //! @param[in] width     Window width.
    //! @param[in] height    Window height.
    //! @param[in] vsync     Manage FPS limit.
    Window(uint32_t width, uint32_t height, bool vsync);
    ~Window();

    //! Main event handle loop. Press ESC to exit.
    //! Present will be called after callback execution.
    void EventLoop(std::function<void()> callback);

    //! Subscribe on window events.
    void AddCallback(IWindowCallback& callback);

    //! Set window title.
    void SetTitle(const std::string& title);

    //! Returns window width.
    uint32_t GetWidth() const;

    //! Returns window height.
    uint32_t GetHeight() const;

private:
    class WindowImpl;
    std::unique_ptr<WindowImpl> mImpl; //!< Hidden GLFW window wrapper
};

};
