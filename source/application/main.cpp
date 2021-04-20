#include "window.h"
#include "renderer.h"
#include "camera.h"

int main()
{
    Engine::Window window(1280, 720);
    Engine::Camera camera(window);
    Engine::Renderer renderer;

    window.EventLoop([&] {
        camera.Update();
        renderer.Render(camera);
    });
    return 0;
}
