#include <SDL2/SDL.h>
#include <iostream>
#include "window/Window.h"
#include "graphics/Renderer.h"

int main(int argc, char* argv[]) {
    Window window;
    Renderer renderer;

    if (!window.Init("GEngine_0b10D", 800, 600, false)) return -1;
    if (!renderer.Init(window.GetSDLWindow())) return -1;

    renderer.SetDrawColor(30, 30, 60, 255);

    while (window.IsRunning()) {
        window.PollEvents();

        renderer.Clear();
        renderer.Present();
    }
    window.Shutdown();
    renderer.Shutdown();

    return 0;
}
