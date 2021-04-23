#pragma once

#include "window.h"
#include "config.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>
#include <set>

namespace Engine
{

//! Simple camera implementation supports movement and rotation.
//! Owns pressed key and forward it to Config.
class Camera : public IWindowCallback
{
public:
    //! Subscribing to window events.
    Camera(Window& window, Config& config);
    ~Camera() override;

    //! Should be called each frame, to handle events
    void Update();

    //! Returns current view*projection matrix, should be called after Update.
    glm::mat4 GetViewProjection() const;

    //! Window keyboard callback.
    void OnKeyEvent(int key, int action) override;

    //! Window mouse callback.
    void OnMouseMove(double x, double y) override;

    //! Returns current config.
    const Config& CurrentConfig() const;

private:
    Window& mWindow;
    Config& mConfig;

    static constexpr double sYAW = -90.0;
    static constexpr double sPITCH = 0.0;
    static constexpr double sSPEED = 2.5;
    static constexpr double sSENSITIVITY = 0.1;
    static constexpr double sZOOM = 45.0;

    double mDeltaTime = 0.0;
    double mLastFrame = 0.0;

    double mLastX = 0.0;
    double mLastY = 0.0;

    bool mFirst = true;
    std::set<int> mPressedKeys;

    class CameraImpl;
    std::unique_ptr<CameraImpl> mImpl;
};

}
