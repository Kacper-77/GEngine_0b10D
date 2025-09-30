#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "core/SystemManager.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "systems/MovementSystem.h"
#include "systems/RenderSystem.h"

#include "window/Window.h"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"
#include "scene/SpriteComponent.h"
#include "input/InputManager.h"

int main(int argc, char* argv[]) {
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image could not initialize! Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    Window window;
    Renderer renderer;

    if (!window.Init("GEngine_ECS_Test", 800, 600, false)) return -1;
    if (!renderer.Init(window.GetSDLWindow())) return -1;

    renderer.SetDrawColor(30, 30, 60, 255);

    // Texture
    Texture playerTexture;
    if (!playerTexture.LoadFromFile("../assets/fish_brown.png", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load player texture." << std::endl;
        return -1;
    }

    // ECS setup
    EntityManager entityManager;

    EntityID player = entityManager.CreateEntity();

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;
    ComponentStorage<SpriteComponent> sprites;

    transforms.Add(player, {368, 268, 64, 64});
    velocities.Add(player, {0.0f, 0.0f});
    accelerations.Add(player, {0.0f, 0.0f});
    sprites.Add(player, {&playerTexture, 64, 64});

    SystemManager systemManager;
    systemManager.RegisterSystem<MovementSystem>(transforms, velocities, accelerations);
    systemManager.RegisterSystem<RenderSystem>(transforms, sprites, &renderer);

    // Input
    InputManager input;
    input.Bind("MoveLeft", SDL_SCANCODE_LEFT);
    input.Bind("MoveRight", SDL_SCANCODE_RIGHT);
    input.Bind("MoveUp", SDL_SCANCODE_UP);
    input.Bind("MoveDown", SDL_SCANCODE_DOWN);

    const float speed = 150.0f; // pixels per second
    Uint32 lastTick = SDL_GetTicks();

    // Main loop
    while (window.IsRunning()) {
        Uint32 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        window.PollEvents();
        input.Update();

        // Update velocity input
        auto* velocity = velocities.Get(player);
        auto* acceleration = accelerations.Get(player);
        if (velocity) {
            if (input.IsActionHeld("MoveLeft"))  velocity->dx -= speed;
            if (input.IsActionHeld("MoveRight")) velocity->dx += speed;
            if (input.IsActionHeld("MoveUp"))    velocity->dy -= speed;
            if (input.IsActionHeld("MoveDown"))  velocity->dy += speed;
        }
        // auto* acceleration = accelerations.Get(player.GetID());
        // velocity->dx += acceleration->ax * deltaTime;

        // Render
        renderer.Clear();
        systemManager.UpdateAll(deltaTime);
        renderer.Present();
    }

    playerTexture.Unload();
    window.Shutdown();
    renderer.Shutdown();
    IMG_Quit();

    return 0;
}
