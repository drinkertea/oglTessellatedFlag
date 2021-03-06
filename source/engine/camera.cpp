#include "camera.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// This class copied as this from LearnOpengGL camra sample
class Camera::CameraImpl
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    double Yaw;
    double Pitch;
    double MovementSpeed;
    double MouseSensitivity;
    double Zoom;

    CameraImpl(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        double yaw = Camera::sYAW,
        double pitch = Camera::sPITCH
    ) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f))
        , MovementSpeed(Camera::sSPEED)
        , MouseSensitivity(Camera::sSENSITIVITY)
        , Zoom(Camera::sZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(CameraMovement direction, double deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == CameraMovement::FORWARD)
            Position += Front * velocity;
        if (direction == CameraMovement::BACKWARD)
            Position -= Front * velocity;
        if (direction == CameraMovement::LEFT)
            Position -= Right * velocity;
        if (direction == CameraMovement::RIGHT)
            Position += Right * velocity;
    }

    void ProcessMouseMovement(double xoffset, double yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

Camera::Camera(Window& window, Config& config)
    : mWindow(window)
    , mConfig(config)
    , mImpl(std::make_unique<CameraImpl>(glm::vec3(0.0f, 0.0f, 3.0f)))
    , mLastX(mWindow.GetWidth() / 2)
    , mLastY(mWindow.GetHeight() / 2)
{
    window.AddCallback(*this);
}

Camera::~Camera()
{
}

void Camera::Update()
{
    double currentFrame = glfwGetTime();
    mDeltaTime = currentFrame - mLastFrame;
    mLastFrame = currentFrame;
    mConfig.xTimeOffset = static_cast<float>(mLastFrame) * mConfig.speed;

    // Multiple keys can be pressed, handle all of it.
    for (auto mLastKey : mPressedKeys)
    {
        if (mLastKey == GLFW_KEY_W)
            mImpl->ProcessKeyboard(CameraMovement::FORWARD, mDeltaTime);
        if (mLastKey == GLFW_KEY_S)
            mImpl->ProcessKeyboard(CameraMovement::BACKWARD, mDeltaTime);
        if (mLastKey == GLFW_KEY_A)
            mImpl->ProcessKeyboard(CameraMovement::LEFT, mDeltaTime);
        if (mLastKey == GLFW_KEY_D)
            mImpl->ProcessKeyboard(CameraMovement::RIGHT, mDeltaTime);
    }
}

glm::mat4 Camera::GetViewProjection() const
{
    glm::mat4 projection = glm::perspective(
        glm::radians(mImpl->Zoom),
        (double)mWindow.GetWidth() / (double)mWindow.GetHeight(),
        0.1,
        100.0
    );
    glm::mat4 view = mImpl->GetViewMatrix();
    return projection * view;
}

void Camera::OnKeyEvent(int key, int action)
{
    if (action == GLFW_RELEASE)
    {
        mPressedKeys.erase(key);
        return;
    }
    if (action != GLFW_PRESS)
    {
        return;
    }

    // Update config.
    if (mConfig.OnKeyPressed(key, mPressedKeys))
    {
        return;
    }

    mPressedKeys.emplace(key);
}

void Camera::OnMouseMove(double x, double y)
{
    if (mFirst)
    {
        mLastX = x;
        mLastY = y;
        mFirst = false;
    }

    double xoffset = x - mLastX;
    double yoffset = mLastY - y;

    mLastX = x;
    mLastY = y;

    mImpl->ProcessMouseMovement(xoffset, yoffset);
}

const Config& Camera::CurrentConfig() const
{
    return mConfig;
}

};
