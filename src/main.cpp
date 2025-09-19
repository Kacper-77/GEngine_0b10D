#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "window/Window.h"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"
#include "scene/Sprite.h"

int main(int argc, char* argv[]) {
    // Init SDL_image
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image could not initialize! Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    Window window;
    Renderer renderer;

    if (!window.Init("GEngine_0b10D", 800, 600, false)) return -1;
    if (!renderer.Init(window.GetSDLWindow())) return -1;

    renderer.SetDrawColor(30, 30, 60, 255);

    // Texture 
    Texture playerTexture;
    if (!playerTexture.LoadFromFile("../assets/fish_brown.png", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load player texture." << std::endl;
        return -1;
    }

    // Sprite
    Sprite player;
    player.SetTexture(&playerTexture);
    player.SetPosition(368, 268);
    player.SetSize(64, 64);

    // Main loop
    while (window.IsRunning()) {
        window.PollEvents();

        renderer.Clear();
        player.Draw(renderer);
        renderer.Present();
    }

    playerTexture.Unload();
    window.Shutdown();
    renderer.Shutdown();
    IMG_Quit();

    return 0;
}
