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
#include "components/CameraComponent.h"

#include "systems/MovementSystem.h"
#include "systems/RenderSystem.h"
#include "systems/EntityCreationSystem.h"
#include "systems/BoundrySystem.h"
#include "systems/CollisionSystem.h"
#include "systems/AudioSystem.h"
#include "systems/CameraSystem.h"

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

    if (SDL_Init(SDL_INIT_AUDIO) < 0 || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    // ECS setup
    EntityManager entityManager;
    AudioSystem audioSystem{&entityManager};
    EntityCreationSystem creationSystem{&entityManager};
    EventBus eventBus;

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;
    ComponentStorage<SpriteComponent> sprites;
    ComponentStorage<BoundryComponent> boundries;
    ComponentStorage<ColliderComponent> colliders;
    ComponentStorage<CameraComponent> cameras;

    // Register component storages
    creationSystem.RegisterStorage(&transforms);
    creationSystem.RegisterStorage(&velocities);
    creationSystem.RegisterStorage(&accelerations);
    creationSystem.RegisterStorage(&sprites);
    creationSystem.RegisterStorage(&boundries);
    creationSystem.RegisterStorage(&colliders);
    creationSystem.RegisterStorage(&cameras);

    entityManager.RegisterComponentStorage(&transforms);
    entityManager.RegisterComponentStorage(&velocities);
    entityManager.RegisterComponentStorage(&accelerations);
    entityManager.RegisterComponentStorage(&sprites);
    entityManager.RegisterComponentStorage(&boundries);
    entityManager.RegisterComponentStorage(&colliders);
    entityManager.RegisterComponentStorage(&cameras);

    // Window and renderer
    Window window;
    Renderer renderer;

    if (!window.Init("GEngine_ECS_Test", 800, 600, false)) return -1;
    if (!renderer.Init(window.GetSDLWindow())) return -1;

    renderer.SetDrawColor(30, 30, 60, 255);

    // Load texture
    Texture playerTexture;
    if (!playerTexture.LoadFromFile("../assets/fish_brown.png", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load player texture." << std::endl;
        return -1;
    }

    // Load sound
    Mix_Chunk* chunk = Mix_LoadWAV("../assets/sample.wav");
    if (!chunk) {
        std::cerr << "Failed to load collision sound: " << Mix_GetError() << std::endl;
        return -1;
    }

    AudioType collisionAudio{AudioType::Type::Chunk, chunk, 5};
    SoundTag collisionTag = "collision";
    audioSystem.RegisterGlobalSound(collisionTag, collisionAudio);

    // Create player
    EntityID player = creationSystem.CreateEntityWith(
        TransformComponent{364, 100, 64, 64},
        VelocityComponent{2.0f, 1.0f},
        AccelerationComponent{0.0f, 0.0f},
        SpriteComponent{&playerTexture, 64, 64},
        BoundryComponent{false, false, false, false},
        ColliderComponent{64, 64, false}
    );

    // Add camera to player
    CameraComponent playerCamera;
    playerCamera.isActive = true;
    playerCamera.viewportSize = {800, 600};
    playerCamera.offset = {0, 0};
    playerCamera.zoom = 1.0f;
    playerCamera.smoothing = 2;
    playerCamera.mode = CameraMode::SmoothFollow;
    playerCamera.target = player;
    cameras.Add(player, playerCamera);

    // Create obstacle
    EntityID blocker = creationSystem.CreateEntityWith(
        TransformComponent{300, 100, 64, 64},
        SpriteComponent{&playerTexture, 64, 64},
        ColliderComponent{64, 64, false}
    );

    // Systems
    MovementSystem movementSystem(transforms, velocities, accelerations);
    // RenderSystem renderSystem(transforms, sprites, &renderer);
    BoundrySystem boundrySystem(transforms, boundries, &window);
    CollisionSystem collisionSystem(entityManager, transforms, colliders, eventBus);
    CameraSystem cameraSystem(transforms, cameras);
    cameraSystem.SetActiveCamera(player);

    SystemManager systemManager;
    systemManager.RegisterSystem<MovementSystem>(movementSystem);
    systemManager.RegisterSystem<BoundrySystem>(boundrySystem);
    systemManager.RegisterSystem<CollisionSystem>(collisionSystem);
    systemManager.RegisterSystem<AudioSystem>(audioSystem);
    systemManager.RegisterSystem<CameraSystem>(cameraSystem);
    systemManager.RegisterSystem<RenderSystem>(transforms, sprites, &renderer);

    // Input
    InputManager inputManager;
    inputManager.Bind("Left", SDL_SCANCODE_LEFT);
    inputManager.Bind("Right", SDL_SCANCODE_RIGHT);
    inputManager.Bind("Up", SDL_SCANCODE_UP);
    inputManager.Bind("Down", SDL_SCANCODE_DOWN);

    // Collision response
    eventBus.Subscribe<CollisionEvent>([&](const CollisionEvent& e) {
        if (e.entityA == player || e.entityB == player) {
            if (auto* velocity = velocities.Get(player)) {
                velocity->dx = 0;
                velocity->dy = 0;
            }
            audioSystem.PlayGlobal(collisionTag);
        }
    });
    auto* renderSystem = systemManager.GetSystem<RenderSystem>();
    // Main loop
    while (window.IsRunning()) {
        window.PollEvents();
        inputManager.Update();

        if (auto* velocity = velocities.Get(player)) {
            velocity->dx = 0;
            velocity->dy = 0;
            if (inputManager.IsActionHeld("Left")) velocity->dx -= 1;
            if (inputManager.IsActionHeld("Right")) velocity->dx += 1;
            if (inputManager.IsActionHeld("Up")) velocity->dy += 5;
            if (inputManager.IsActionHeld("Down")) velocity->dy -= 1;
        }
        
        systemManager.UpdateAll(1.0f);
        cameraSystem.ApplyToRenderSystem(*renderSystem);

        eventBus.Dispatch();

        renderer.Clear();
        renderSystem->Update(1.0f);
        renderer.Present();

        SDL_Delay(1000 / 60);
    }

    playerTexture.Unload();
    window.Shutdown();
    renderer.Shutdown();
    IMG_Quit();
    return 0;
}
