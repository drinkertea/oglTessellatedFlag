#pragma once

#include "window.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>
#include <set>

namespace Engine
{

struct Config
{
    float amplitude   = 0.1f;
    float waveCount   = 1.7f;
    float xBaseOffset = -0.5f;
    float xTimeOffset = 0.0f;
    float speed       = 0.5f;
    uint32_t depth    = 20;
};

class Camera : public IWindowCallback
{
public:
    Camera(Window& window);
    ~Camera() override;

    void Update();
    glm::mat4 GetViewProjection() const;

    void OnKeyPressed(int key, int action) override;
    void OnMouseMove(double x, double y) override;

    const Config& CurrentConfig() const;

private:
    Window& mWindow;

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

    Config mConfig{};

    class CameraImpl;
    std::unique_ptr<CameraImpl> mImpl;
};

}
