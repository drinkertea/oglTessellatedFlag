#include "include/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

namespace Engine
{
class Window::WindowImpl
{
public:
    WindowImpl(uint32_t width, uint32_t height)
        : mWidth(width)
        , mHeight(height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        mWindow = glfwCreateWindow(width, height, "OpenGL Tessellation Sample", nullptr, nullptr);
        if (!mWindow)
        {
            throw std::runtime_error("Unable to create GLFW window.");
        }
        glfwMakeContextCurrent(mWindow);
        glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD.");
        }

        glfwSetWindowUserPointer(mWindow, reinterpret_cast<void*>(this));

        glfwSetCursorPosCallback(mWindow, MouseCallback);
        glfwSetKeyCallback(mWindow, KeyCallback);
    }

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static WindowImpl& GetWindow(GLFWwindow* window)
    {
        WindowImpl* thisWindow = reinterpret_cast<WindowImpl*>(glfwGetWindowUserPointer(window));
        if (!thisWindow)
        {
            throw std::runtime_error("No window instance assotiated with GLFW window.");
        }
        return *thisWindow;
    }

    static void MouseCallback(GLFWwindow* window, double x, double y)
    {
        for (auto& callback : GetWindow(window).GetCallbacks())
        {
            callback.get().OnMouseMove(x, y);
        }
    }

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_K && action == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (key == GLFW_KEY_L && action == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
            return;
        }

        for (auto& callback : GetWindow(window).GetCallbacks())
        {
            callback.get().OnKeyPressed(key, action);
        }
    }

    void WindowImpl::EventLoop(std::function<void()> callback)
    {
        while (!glfwWindowShouldClose(mWindow))
        {
            callback();
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
        }
    }

    ~WindowImpl()
    {
        glfwTerminate();
    }

    const std::vector<IWindowCallback::Ref>& GetCallbacks() const
    {
        return mCallbacks;
    }

    void AddCallback(IWindowCallback& callback)
    {
        mCallbacks.push_back(callback);
    }

    uint32_t GetWidth() const
    {
        return mWidth;
    }

    uint32_t GetHeight() const
    {
        return mHeight;
    }

private:
    GLFWwindow* mWindow = nullptr;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;

    std::vector<IWindowCallback::Ref> mCallbacks;
};

Window::Window(uint32_t width, uint32_t height)
    : mImpl(std::make_unique<WindowImpl>(width, height))
{
}

Window::~Window() = default;

void Window::EventLoop(std::function<void()> callback)
{
    mImpl->EventLoop(std::move(callback));
}

void Window::AddCallback(IWindowCallback& callback)
{
    mImpl->AddCallback(callback);
}

uint32_t Window::GetWidth() const
{
    return mImpl->GetHeight();
}

uint32_t Window::GetHeight() const
{
    return mImpl->GetHeight();
}

};
