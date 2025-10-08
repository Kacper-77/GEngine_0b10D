#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "core/SystemManager.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/AccelerationComponent.h"
#include "components/HealthComponent.h"
#include "components/BoundryComponent.h"
#include "components/ColliderComponent.h"
#include "components/SpriteComponent.h"

#include "systems/MovementSystem.h"
#include "systems/RenderSystem.h"
#include "systems/EntityCreationSystem.h"
#include "systems/BoundrySystem.h"
#include "systems/CollisionSystem.h"

#include "window/Window.h"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"
#include "input/InputManager.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"

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
    EventBus eventBus;

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;
    ComponentStorage<SpriteComponent> sprites;
    ComponentStorage<BoundryComponent> boundries;
    ComponentStorage<ColliderComponent> colliders;

    creationSystem.RegisterStorage(&transforms);
    creationSystem.RegisterStorage(&velocities);
    creationSystem.RegisterStorage(&accelerations);
    creationSystem.RegisterStorage(&sprites);
    creationSystem.RegisterStorage(&boundries);
    creationSystem.RegisterStorage(&colliders);

    entityManager.RegisterComponentStorage(&transforms);
    entityManager.RegisterComponentStorage(&velocities);
    entityManager.RegisterComponentStorage(&accelerations);
    entityManager.RegisterComponentStorage(&sprites);
    entityManager.RegisterComponentStorage(&boundries);
    entityManager.RegisterComponentStorage(&colliders);

    // Load texture
    Texture playerTexture;
    if (!playerTexture.LoadFromFile("../assets/fish_brown.png", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load player texture." << std::endl;
        return -1;
    }

    // Create player entity
    EntityID player = creationSystem.CreateEntityWith(
        TransformComponent{364, 100, 64, 64},
        VelocityComponent{2.0f, 1.0f},
        AccelerationComponent{0.0f, 0.0f},
        SpriteComponent{&playerTexture, 64, 64},
        BoundryComponent{true, true, true, true},
        ColliderComponent{64, 64, false}
    );

    // Create static obstacle
    EntityID blocker = creationSystem.CreateEntityWith(
        TransformComponent{300, 100, 64, 64},
        SpriteComponent{&playerTexture, 64, 64},
        ColliderComponent{64, 64, false}
    );

    // Systems
    MovementSystem movementSystem(transforms, velocities, accelerations);
    RenderSystem renderSystem(transforms, sprites, &renderer);
    BoundrySystem boundrySystem(transforms, boundries, &window);
    CollisionSystem collisionSystem(entityManager, transforms, colliders, eventBus);

    SystemManager systemManager;
    systemManager.RegisterSystem<MovementSystem>(movementSystem);
    systemManager.RegisterSystem<BoundrySystem>(boundrySystem);
    systemManager.RegisterSystem<CollisionSystem>(collisionSystem);

    // Input
    InputManager inputManager;
    inputManager.Bind("Left", SDL_SCANCODE_LEFT);
    inputManager.Bind("Right", SDL_SCANCODE_RIGHT);
    inputManager.Bind("Up", SDL_SCANCODE_UP);
    inputManager.Bind("Down", SDL_SCANCODE_DOWN);


    // Collision response (simple stop)
    eventBus.Subscribe<CollisionEvent>([&](const CollisionEvent& e) {
        if (e.entityA == player || e.entityB == player) {
            auto* velocity = velocities.Get(player);
            if (velocity) {
                velocity->dx = 0;
                velocity->dy = 0;
            }
        }
    });

    while (window.IsRunning()) {
        window.PollEvents();
        inputManager.Update();

        auto velocity = velocities.Get(player);
        if (inputManager.IsActionHeld("Left")) velocity->dx -= 1;
        if (inputManager.IsActionHeld("Right")) velocity->dx += 1;
        if (inputManager.IsActionHeld("Up")) velocity->dy -= 1;
        if (inputManager.IsActionHeld("Down")) velocity->dy += 1;

        systemManager.UpdateAll(1.0f);
        eventBus.Dispatch();
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
