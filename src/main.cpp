#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "core/SystemManager.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/HealthComponent.h"
#include "systems/MovementSystem.h"
#include "systems/RenderSystem.h"
#include "systems/EntityCreationSystem.h"

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

    // ECS setup
    EntityManager entityManager;
    EntityCreationSystem creationSystem{&entityManager};

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;
    ComponentStorage<SpriteComponent> sprites;

    creationSystem.RegisterStorage(&transforms);
    creationSystem.RegisterStorage(&velocities);
    creationSystem.RegisterStorage(&accelerations);
    creationSystem.RegisterStorage(&sprites);

    entityManager.RegisterComponentStorage(&transforms);
    entityManager.RegisterComponentStorage(&velocities);
    entityManager.RegisterComponentStorage(&accelerations);
    entityManager.RegisterComponentStorage(&sprites);

    // Load texture
    Texture playerTexture;
    if (!playerTexture.LoadFromFile("../assets/fish_brown.png", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load player texture." << std::endl;
        return -1;
    }

    // Create entity
    EntityID player = creationSystem.CreateEntityWith(
        TransformComponent{364, 100, 64, 64},
        VelocityComponent{2.0f, 1.0f},
        AccelerationComponent{0.0f, 0.0f},
        SpriteComponent{&playerTexture, 64, 64}
    );

    MovementSystem movementSystem(transforms, velocities, accelerations);
    RenderSystem renderSystem(transforms, sprites, &renderer);
    SystemManager systemManager;
    systemManager.RegisterSystem<MovementSystem>(movementSystem);

    InputManager inputManager;
    inputManager.Bind("Left", SDL_SCANCODE_LEFT);

    auto transform = transforms.Get(player);

    while (window.IsRunning()) {
        window.PollEvents();
        inputManager.Update();

        auto velocity = velocities.Get(player);

        if (inputManager.IsActionHeld("Left")) velocity->dx -= 1;

        systemManager.UpdateAll(1.0f);
        renderer.Clear();
        renderSystem.Update(1.0f);
        renderer.Present();

        SDL_Delay(1000 / 60); // ~60 FPS
    }

    playerTexture.Unload();
    window.Shutdown();
    renderer.Shutdown();
    IMG_Quit();

    return 0;
}
