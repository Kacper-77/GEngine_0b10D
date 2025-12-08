#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"
#include "core/ComponentStorage.h"
#include "core/SystemManager.h"
#include "AI/AISystem.h"
#include "event/core/EventBus.h"

#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/AccelerationComponent.h"
#include "components/SpriteComponent.h"
#include "components/CameraComponent.h"
#include "components/BoundryComponent.h"
#include "components/ColliderComponent.h"
#include "components/SurfaceComponent.h"
#include "utils/AnimationUtils.h"
#include "components/PhysicsComponent.h"

#include "systems/MovementSystem.h"
#include "systems/RenderSystem.h"
#include "systems/BoundrySystem.h"
#include "systems/CollisionSystem.h"
#include "systems/CameraSystem.h"
#include "systems/SurfaceBehaviorSystem.h"
#include "systems/AnimationSystem.h"
#include "systems/PhysicsSystem.h"

#include "window/Window.h"
#include "graphics/Renderer.h"
#include "input/InputManager.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"
#include "utils/SpatialGrid.h"

#include "level_manager/AssetManager.h"
#include "level_manager/ResourceLoader.h"

int main(int argc, char* argv[]) {
    if (IMG_Init(IMG_INIT_PNG) == 0 || SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    // ECS 
    EntityManager entityManager;
    EntityCreationSystem creationSystem{&entityManager};

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<VelocityComponent> velocities;
    ComponentStorage<AccelerationComponent> accelerations;
    ComponentStorage<SpriteComponent> sprites;
    ComponentStorage<CameraComponent> cameras;
    ComponentStorage<BoundryComponent> boundaries;
    ComponentStorage<ColliderComponent> colliders;
    ComponentStorage<SurfaceComponent> surfaces;
    ComponentStorage<AnimationComponent> animations;
    ComponentStorage<PhysicsComponent> physics;

    // Register storages
    creationSystem.RegisterStorage(&transforms);
    creationSystem.RegisterStorage(&velocities);
    creationSystem.RegisterStorage(&accelerations);
    creationSystem.RegisterStorage(&sprites);
    creationSystem.RegisterStorage(&cameras);
    creationSystem.RegisterStorage(&boundaries);
    creationSystem.RegisterStorage(&colliders);
    creationSystem.RegisterStorage(&surfaces);
    creationSystem.RegisterStorage(&animations);
    creationSystem.RegisterStorage(&physics);

    entityManager.RegisterComponentStorage(&transforms);
    entityManager.RegisterComponentStorage(&velocities);
    entityManager.RegisterComponentStorage(&accelerations);
    entityManager.RegisterComponentStorage(&sprites);
    entityManager.RegisterComponentStorage(&cameras);
    entityManager.RegisterComponentStorage(&boundaries);
    entityManager.RegisterComponentStorage(&colliders);
    entityManager.RegisterComponentStorage(&surfaces);
    entityManager.RegisterComponentStorage(&animations);
    entityManager.RegisterComponentStorage(&physics);

    // Window + Renderer
    Window window;
    Renderer renderer;
    if (!window.Init("Game", 1200, 720, false)) return -1;
    if (!renderer.Init(window.GetSDLWindow())) return -1;

    // Asset Manager + Loader + required Systems
    AssetManager assets(&renderer);
    AISystem ai;
    EventBus eventBus;

    ResourceLoader loader(
        &renderer,
        &creationSystem,
        &assets,
        &entityManager,
        &eventBus,
        &ai,
        &transforms,
        &boundaries,
        &sprites,
        &physics,
        &colliders,
        &cameras,
        &surfaces,
        &velocities,
        &accelerations,
        &animations
    );

    // Load Scene
    if (!loader.LoadScene("../assets/scene.json")) {
        std::cerr << "Failed to load scene\n";
        return -1;
    }

    // Systems
    SystemManager systemManager;

    MovementSystem movementSystem(transforms, velocities, accelerations);
    CameraSystem cameraSystem(transforms, cameras);
    AnimationSystem animationSystem(animations, sprites, transforms);
    BoundrySystem boundrySystem(transforms, boundaries, &window);
    CollisionSystem collisionSystem(entityManager, transforms, colliders);
    PhysicsSystem physicsSystem(transforms, accelerations, physics);

    SpatialGrid<EntityID> spatialGrid;
    SurfaceBehaviorSystem sbh(transforms, velocities, surfaces, spatialGrid);

    RenderSystem renderSystem(transforms, sprites, &renderer);

    systemManager.RegisterSystem<MovementSystem>(movementSystem);
    systemManager.RegisterSystem<CameraSystem>(cameraSystem);
    systemManager.RegisterSystem<AnimationSystem>(animationSystem);
    systemManager.RegisterSystem<BoundrySystem>(boundrySystem);
    systemManager.RegisterSystem<CollisionSystem>(collisionSystem);
    systemManager.RegisterSystem<PhysicsSystem>(physicsSystem);
    systemManager.RegisterSystem<SurfaceBehaviorSystem>(sbh);
    systemManager.RegisterSystem<AISystem>(ai);

    // Texture background;
    // background.LoadFromFile("../assets/background.jpeg", renderer.GetSDLRenderer());
    // renderSystem.AddBackgroundLayer(&background, 0.0f);
    renderSystem.AddBackgroundLayer(assets.GetTexture("background"), 0.0f);


    // Input
    InputManager input;
    input.Bind("Left", SDL_SCANCODE_LEFT);
    input.Bind("Right", SDL_SCANCODE_RIGHT);
    input.Bind("Up", SDL_SCANCODE_UP);

    // Main Loop
    const float dt = 1.0f / 60.0f;

    // Player loaded from JSON by tag
    EntityID player = entityManager.GetGroup("player").empty()
                      ? INVALID_ENTITY
                      : *entityManager.GetGroup("player").begin();

    while (window.IsRunning()) {
        window.PollEvents();
        input.Update();

        // Player movement
        if (auto* velocity = velocities.Get(player)) {
            if (input.IsActionHeld("Left"))  velocity->dx -= 1;
            if (input.IsActionHeld("Right")) velocity->dx += 1;
            if (input.IsActionHeld("Up")) {
                if (auto* phys = physics.Get(player)) {
                    phys->impulse.y = -30.0f;
                }
            }
        }

        systemManager.UpdateAll(dt);
        animationSystem.Update(dt);
        cameraSystem.ApplyToRenderSystem(renderSystem);
        ai.Update(100.0f);

        renderer.Clear();
        renderSystem.Update(dt);
        renderer.Present();

        SDL_Delay(1000 / 60);
    }

    window.Shutdown();
    renderer.Shutdown();
    IMG_Quit();
    return 0;
}
