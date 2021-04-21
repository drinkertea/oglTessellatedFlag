#include "window.h"
#include "renderer.h"
#include "camera.h"
#include "config.h"
#include "counter.h"

#include <stdexcept>
#include <iostream>

int main()
{
    try
    {
        Engine::Window   window{ 1280, 720, false };
        Engine::Config   config{};
        Engine::Camera   camera{ window, config };
        Engine::Renderer renderer{};
        Engine::Counter  counter{};

        window.EventLoop([&] {
            camera.Update();
            renderer.Render(camera);
            counter.Update();
            window.SetTitle(counter.GetPresentation() + config.ToString());
        });
    }
    catch (const std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl;
        return -1;
    }
    return 0;
}
