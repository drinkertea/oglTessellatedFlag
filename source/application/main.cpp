#include "window.h"
#include "renderer.h"

int main()
{
    Engine::Window window(1280, 720);
    Engine::Renderer renderer;

    window.EventLoop([&] {
    });
    return 0;
}
