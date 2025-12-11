#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"
#include "core/ComponentStorage.h"
#include "core/SystemManager.h"
#include "AI/AISystem.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"

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
#include "systems/AudioSystem.h"
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

void DrawGrid(SDL_Renderer* sdlRenderer, const SDL_Point& camPos, int width, int height, int cellSize = 64) {
    SDL_SetRenderDrawColor(sdlRenderer, 80, 80, 80, 255);

    for (int x = - (camPos.x % cellSize); x < width; x += cellSize) {
        SDL_RenderDrawLine(sdlRenderer, x, 0, x, height);
    }

    for (int y = - (camPos.y % cellSize); y < height; y += cellSize) {
        SDL_RenderDrawLine(sdlRenderer, 0, y, width, y);
    }
}

int main(int argc, char* argv[]) {
    if (IMG_Init(IMG_INIT_PNG) == 0 || SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer init failed: " << Mix_GetError() << std::endl;
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
    RenderSystem renderSystem(transforms, sprites, &renderer);

    systemManager.RegisterSystem<MovementSystem>(transforms, velocities, accelerations, physics);
    systemManager.RegisterSystem<CameraSystem>(transforms, cameras);
    systemManager.RegisterSystem<AudioSystem>(&entityManager);
    systemManager.RegisterSystem<AnimationSystem>(animations, sprites, transforms);
    systemManager.RegisterSystem<CollisionSystem>(entityManager, transforms, colliders);
    systemManager.RegisterSystem<PhysicsSystem>(transforms, accelerations, physics);
    systemManager.RegisterSystem<BoundrySystem>(transforms, boundaries, physics, &window);
    SpatialGrid<EntityID> spatialGrid;
    systemManager.RegisterSystem<SurfaceBehaviorSystem>(transforms, velocities, surfaces, physics, spatialGrid);
    systemManager.RegisterSystem<AISystem>(ai);


    renderSystem.AddBackgroundLayer(assets.GetTexture("background"), 0.0f);

    // Audio
    auto* audioSystem = systemManager.GetSystem<AudioSystem>();

    AudioType ocean;
    ocean.type = AudioType::Type::Chunk;
    ocean.chunk = Mix_LoadWAV("../assets/ocean.wav");

    AudioType theme;
    theme.type = AudioType::Type::Music;
    theme.music = Mix_LoadMUS("../assets/theme.mp3");

    AudioType bounce;
    bounce.type = AudioType::Type::Chunk;
    bounce.chunk = Mix_LoadWAV("../assets/bounce.wav");

    audioSystem->RegisterGlobalSound("Theme", theme);
    audioSystem->RegisterGlobalSound("Ocean", ocean);
    audioSystem->RegisterGlobalSound("Bounce", bounce);
    audioSystem->PlayGlobal("Ocean");
    audioSystem->PlayGlobal("Theme");
    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.5);

    // Input
    InputManager input;
    input.Bind("Left", SDL_SCANCODE_LEFT);
    input.Bind("Right", SDL_SCANCODE_RIGHT);
    input.Bind("Up", SDL_SCANCODE_UP);
    input.Bind("Down", SDL_SCANCODE_DOWN);

    // Main Loop
    const float dt = 1.0f / 60.0f;

    // Player loaded from JSON by tag
    EntityID player = entityManager.GetGroup("player").empty()
                      ? INVALID_ENTITY
                      : *entityManager.GetGroup("player").begin();

    auto* npc = ai.GetController(1);
    auto* transform = transforms.Get(player);

    auto* cam = systemManager.GetSystem<CameraSystem>();
    auto* phys = systemManager.GetSystem<PhysicsSystem>();

    phys->SetGravity(90.00f);
    
    cam->SetActiveCamera(player);
    cam->FocusOn(player);
    eventBus.Subscribe<CollisionEvent>(
        [&](const CollisionEvent& e) {

            if ((e.entityA == 1 && e.entityB == 4) ||
                (e.entityA == 4 && e.entityB == 1))
            {
                auto* phys = physics.Get(1);
                if (!phys) return;
                
                phys->impulse.y -= 20;
                int ch = Mix_PlayChannel(-1, bounce.chunk, 0);
                Mix_Volume(ch, MIX_MAX_VOLUME * 0.2);
            }
        }
    );

    auto* collisionSystem = systemManager.GetSystem<CollisionSystem>();

    while (window.IsRunning()) {
        window.PollEvents();
        input.Update();

        // Player movement
        if (auto* velocity = physics.Get(player)) {
            if (input.IsActionHeld("Left"))  velocity->impulse.x -= 10;
            if (input.IsActionHeld("Right")) velocity->impulse.x += 10;
            if (input.IsActionHeld("Up")) velocity->impulse.y -= 10;
            if (input.IsActionHeld("Down")) velocity->impulse.y += 10;
        }

        systemManager.UpdateAll(dt);
        for (auto& [a, b] : collisionSystem->GetCollisions()) {
            eventBus.PublishImmediate(CollisionEvent(a, b, "", ""));
        }

        cam->ApplyToRenderSystem(renderSystem);
        ai.Update(dt);

        renderer.Clear();
        renderSystem.Update(dt);

        SDL_Point camPos = renderSystem.GetCameraPosition();
        DrawGrid(renderer.GetSDLRenderer(), camPos, 1200, 720, 64);

        renderer.Present();

        SDL_Delay(1000 / 60);
    }

    window.Shutdown();
    renderer.Shutdown();
    IMG_Quit();
    return 0;
}
