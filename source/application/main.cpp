#include "window.h"

int main()
{
    Engine::Window window(1280, 720);
    window.EventLoop([&] {
    });
    return 0;
}
