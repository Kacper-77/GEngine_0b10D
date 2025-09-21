#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "window/Window.h"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"
#include "scene/Sprite.h"
#include "input/InputManager.h"

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
    player.SetSize(64, 64);

    int playerX = 368;
    int playerY = 268;
    player.SetPosition(playerX, playerY);

    // Input
    InputManager input;
    input.Bind("MoveLeft", SDL_SCANCODE_LEFT);
    input.Bind("MoveRight", SDL_SCANCODE_RIGHT);
    input.Bind("MoveUp", SDL_SCANCODE_UP);
    input.Bind("MoveDown", SDL_SCANCODE_DOWN);

    const int speed = 4;

    // Main loop
    while (window.IsRunning()) {
        window.PollEvents();
        input.Update();

        // Movement
        if (input.IsActionHeld("MoveLeft"))  playerX -= speed;
        if (input.IsActionHeld("MoveRight")) playerX += speed;
        if (input.IsActionHeld("MoveUp"))    playerY -= speed;
        if (input.IsActionHeld("MoveDown"))  playerY += speed;

        player.SetPosition(playerX, playerY);

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
