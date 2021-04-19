#include "include/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Engine
{
class Window::WindowImpl
{
public:
    WindowImpl(uint32_t width, uint32_t height)
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

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD.");
        }
    }

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
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

private:
    GLFWwindow* mWindow = nullptr;
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

};
