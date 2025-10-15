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
    if (IMG_Init(IMG_INIT_PNG) == 0 || SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    // ECS setup
    EntityManager entityManager;
    EntityCreationSystem creationSystem{&entityManager};

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;
    ComponentStorage<SpriteComponent> sprites;
    ComponentStorage<CameraComponent> cameras;

    creationSystem.RegisterStorage(&transforms);
    creationSystem.RegisterStorage(&velocities);
    creationSystem.RegisterStorage(&accelerations);
    creationSystem.RegisterStorage(&sprites);
    creationSystem.RegisterStorage(&cameras);

    entityManager.RegisterComponentStorage(&transforms);
    entityManager.RegisterComponentStorage(&velocities);
    entityManager.RegisterComponentStorage(&accelerations);
    entityManager.RegisterComponentStorage(&sprites);
    entityManager.RegisterComponentStorage(&cameras);

    // Window and renderer
    Window window;
    Renderer renderer;
    if (!window.Init("Minimal ECS Test", 800, 600, false)) return -1;
    if (!renderer.Init(window.GetSDLWindow())) return -1;
    renderer.SetDrawColor(30, 30, 60, 255);

    // Load texture
    Texture texture;
    if (!texture.LoadFromFile("../assets/fish_brown.png", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load texture." << std::endl;
        return -1;
    }

    // Create player
    EntityID player = creationSystem.CreateEntityWith(
        TransformComponent{364, 100, 64, 64},
        VelocityComponent{0.0f, 0.0f},
        AccelerationComponent{0.0f, 0.0f},
        SpriteComponent{&texture, 64, 64}
    );

    // Add camera to player
    CameraComponent camera;
    camera.isActive = true;
    camera.viewportSize = {800, 600};
    camera.offset = {0, 0};
    camera.zoom = 1.0f;
    camera.mode = CameraMode::Follow;
    camera.target = player;
    cameras.Add(player, camera);

    // Create blocker
    EntityID blocker = creationSystem.CreateEntityWith(
        TransformComponent{300, 100, 64, 64},
        SpriteComponent{&texture, 64, 64}
    );

    // Systems
    MovementSystem movementSystem(transforms, velocities, accelerations);
    CameraSystem cameraSystem(transforms, cameras);
    cameraSystem.SetActiveCamera(player);

    SystemManager systemManager;
    systemManager.RegisterSystem<MovementSystem>(movementSystem);
    systemManager.RegisterSystem<CameraSystem>(cameraSystem);

    RenderSystem renderSystem(transforms, sprites, &renderer);

    auto* position = transforms.Get(player);

    // Input
    InputManager input;
    input.Bind("Left", SDL_SCANCODE_LEFT);
    input.Bind("Right", SDL_SCANCODE_RIGHT);

    // Main loop
    while (window.IsRunning()) {
        window.PollEvents();
        input.Update();

        if (auto* velocity = velocities.Get(player)) {
            velocity->dx = 0;
            if (input.IsActionHeld("Left")) {
                velocity->dx -= 1;
            }
            if (input.IsActionHeld("Right")) {
                velocity->dx += 1;
            }
        }
        
        cameraSystem.Update(1.0f);
        cameraSystem.ApplyToRenderSystem(renderSystem);
        systemManager.UpdateAll(1.0f);
        
        renderer.Clear();
        renderSystem.Update(1.0f);
        renderer.Present();

        SDL_Delay(1000 / 60);
    }

    texture.Unload();
    window.Shutdown();
    renderer.Shutdown();
    IMG_Quit();
    return 0;
}
