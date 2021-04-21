#include "window.h"
#include "renderer.h"
#include "camera.h"
#include "config.h"
#include "counter.h"

int main()
{
    Engine::Window window(1280, 720, false);
    Engine::Config config{};
    Engine::Camera camera(window, config);
    Engine::Renderer renderer;

    Engine::Counter counter;
    window.EventLoop([&] {
        camera.Update();
        renderer.Render(camera);
        counter.Update();
        window.SetTitle(counter.GetPresentation() + config.ToString());
    });
    return 0;
}
