#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>

static constexpr float PI = 3.14159265358979323846f;

#include "AI/AISystem.h"
#include "AI/AIController.h"
#include "AI/behaviors/PatrolBehavior.h"

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
#include "components/SurfaceComponent.h"

#include "systems/MovementSystem.h"
#include "systems/RenderSystem.h"
#include "systems/EntityCreationSystem.h"
#include "systems/BoundrySystem.h"
#include "systems/CollisionSystem.h"
#include "systems/AudioSystem.h"
#include "systems/CameraSystem.h"
#include "systems/SurfaceBehaviorSystem.h"
#include "systems/AnimationSystem.h"

#include "window/Window.h"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"
#include "input/InputManager.h"
#include "event/core/EventBus.h"
#include "event/custom_events/CollisionEvent.h"
#include "utils/SpatialGrid.h"
#include "utils/AnimationUtils.h"

void DrawArm(Renderer* renderer,
             const TransformComponent& body,
             const TransformComponent& arm,
             const SDL_Point& camPos,
             bool isLeft) {
    renderer->SetDrawColor(0, 0, 0, 255);

    int cx = static_cast<int>(body.position.x - camPos.x);
    int cy = static_cast<int>(body.position.y - camPos.y);

    int anchorX = cx;
    int anchorY = cy + 11;

    float length = 20.0f;

    float baseAngle = isLeft ? 180.0f : 0.0f;
    float rad = (arm.rotationDeg + baseAngle) * PI / 180.0f;

    int endX = anchorX + static_cast<int>(std::cos(rad) * length);
    int endY = anchorY + static_cast<int>(std::sin(rad) * length);

    renderer->DrawLine(anchorX, anchorY, endX, endY);
}

void DrawStickman(Renderer* renderer,
                  const TransformComponent& body,
                  const TransformComponent* leftArm,
                  const TransformComponent* rightArm,
                  const SDL_Point& camPos) {
    renderer->SetDrawColor(0, 0, 0, 255);

    int headRadius = 10;
    int cx = static_cast<int>(body.position.x - camPos.x);
    int cy = static_cast<int>(body.position.y - camPos.y);

    // Head
    for (int w = -headRadius; w <= headRadius; ++w) {
        for (int h = -headRadius; h <= headRadius; ++h) {
            if (w*w + h*h <= headRadius*headRadius) {
                renderer->DrawPoint(cx + w, cy + h);
            }
        }
    }

    // Arms: draw with explicit side anchors
    if (leftArm)  DrawArm(renderer, body, *leftArm, camPos, true);
    if (rightArm) DrawArm(renderer, body, *rightArm, camPos, false);

    // Body
    renderer->DrawLine( cx, cy + headRadius, cx, cy + 40);

    // Legs
    renderer->DrawLine(cx, cy + 40, cx - 15, cy + 60);
    renderer->DrawLine(cx, cy + 40, cx + 15, cy + 60);

}


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

    // ECS setup
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

    creationSystem.RegisterStorage(&transforms);
    creationSystem.RegisterStorage(&velocities);
    creationSystem.RegisterStorage(&accelerations);
    creationSystem.RegisterStorage(&sprites);
    creationSystem.RegisterStorage(&cameras);
    creationSystem.RegisterStorage(&boundaries);
    creationSystem.RegisterStorage(&colliders);
    creationSystem.RegisterStorage(&surfaces);
    creationSystem.RegisterStorage(&animations);

    entityManager.RegisterComponentStorage(&transforms);
    entityManager.RegisterComponentStorage(&velocities);
    entityManager.RegisterComponentStorage(&accelerations);
    entityManager.RegisterComponentStorage(&sprites);
    entityManager.RegisterComponentStorage(&cameras);
    entityManager.RegisterComponentStorage(&boundaries);
    entityManager.RegisterComponentStorage(&colliders);
    entityManager.RegisterComponentStorage(&surfaces);
    entityManager.RegisterComponentStorage(&animations);

    // Window and renderer
    Window window;
    Renderer renderer;
    if (!window.Init("Minimal ECS Test", 1200, 720, true)) return -1;
    if (!renderer.Init(window.GetSDLWindow())) return -1;

    // Load texture
    Texture texture;
    if (!texture.LoadFromFile("../assets/fish_brown.png", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load texture." << std::endl;
        return -1;
    }

    Texture platform;
    if (!platform.LoadFromFile("../assets/platforms.png", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load texture." << std::endl;
        return -1;
    }

    // Create player
    EntityID player = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{364.0f, 200.0f}, 0.0f, VectorFloat{128.0f, 128.0f} },
        ColliderComponent{64, 64, CollisionLayer::Player, CollisionLayer::Enemy},
        VelocityComponent{0.0f, 0.0f},
        AccelerationComponent{0.0f, 0.0f},
        SpriteComponent{&texture, 64, 64},
        BoundryComponent{true, true, true, true}
    );

    EntityID grass = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{364.0f, 280.0f}, 0.0f, VectorFloat{200.0f, 40.0f} },
        SpriteComponent{&platform, 200, 40},
        SurfaceComponent{364.0f, 280.0f, 200.0f, 40.0f, SurfaceType::SAND}
    );

    entityManager.AddTag(grass, "grass");

    // Add camera to player
    CameraComponent camera;
    camera.isActive = true;
    camera.viewportSize = {800, 600};
    camera.offset = {100, 0};
    camera.zoom = 1.0f;
    camera.mode = CameraMode::Follow;
    camera.target = player;

    camera.position = {0, 0};
    camera.rotationDegrees = 0.0f;
    camera.fadeAlpha = 255;
    camera.useBounds = false;
    camera.bounds = {0, 0, 0, 0};
    camera.smoothing = 8.0f;
    camera.manualControl = false;
    camera.shakeIntensity = 0;
    camera.shakeDuration = 0.0f;
    cameras.Add(player, camera);

    // Create blocker
    EntityID blocker = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{600.0f, 300.0f}, 0.0f, VectorFloat{64.0f, 64.0f} },
        SpriteComponent{&texture, 64, 64},
        VelocityComponent{0.0f, 0.0f},
        ColliderComponent{64, 64, CollisionLayer::Enemy, CollisionLayer::Projectile}
    );

    AIController blockerAI(100, 100);
    blockerAI.SetPosition({600, 300});
    blockerAI.ChangeState(AIState::Patrol);
    auto* blockerTransform = transforms.Get(blocker);
    auto* blockerVelocity = velocities.Get(blocker);
    blockerAI.AttachComponents(blockerTransform, blockerVelocity);

    // Define patrol route
    blockerAI.SetPatrolRoute({
        {600, 300},
        {700, 300},
        {700, 400},
        {600, 400}
    });

    // Attach PatrolBehavior
    blockerAI.SetBehavior(std::make_unique<PatrolBehavior>());

    // Add to AISystem
    AISystem aiSystem;
    aiSystem.AddController(&blockerAI);

    // Animations: stickman body and arms
    EntityID stickmanEntity = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{400.0f, 200.0f}, 0.0f, VectorFloat{1.0f, 1.0f} }
    );

    EntityID leftArmEntity  = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{-10.0f, 0.0f}, 0.0f, VectorFloat{1.0f, 1.0f} }
    );
    EntityID rightArmEntity = creationSystem.CreateEntityWith(
        TransformComponent{ VectorFloat{ 10.0f, 0.0f}, 0.0f, VectorFloat{1.0f, 1.0f} }
    );

    // Keyframes
    BoneAnimation leftAnim;
    leftAnim.boneID = leftArmEntity;
    leftAnim.frames = {
        {0.0f, AnimationTransform{ {0,0}, -30.0f, {1,1} }},
        {0.5f, AnimationTransform{ {0,0},  30.0f, {1,1} }},
        {1.0f, AnimationTransform{ {0,0}, -30.0f, {1,1} }}
    };

    SkeletalAnimationClip leftClip;
    leftClip.duration = 1.0f;
    leftClip.loop = true;
    leftClip.bones = { leftAnim };

    BoneAnimation rightAnim;
    rightAnim.boneID = rightArmEntity;
    rightAnim.frames = {
        {0.0f, AnimationTransform{ {0,0},  30.0f, {1,1} }},
        {0.5f, AnimationTransform{ {0,0}, -30.0f, {1,1} }},
        {1.0f, AnimationTransform{ {0,0},  30.0f, {1,1} }}
    };
    
    SkeletalAnimationClip rightClip;
    rightClip.duration = 1.0f;
    rightClip.loop = true;
    rightClip.bones = { rightAnim };

    AnimationLayer leftLayer;
    leftLayer.type = AnimationType::SKELETAL;
    leftLayer.clip = &leftClip;
    leftLayer.loop = true;
    leftLayer.weight = 1.0f;

    AnimationLayer rightLayer;
    rightLayer.type = AnimationType::SKELETAL;
    rightLayer.clip = &rightClip;
    rightLayer.loop = true;
    rightLayer.weight = 1.0f;

    AnimationComponent leftComp;
    leftComp.baseLayer = leftLayer;
    leftComp.speed = 1.0f;
    animations.Add(leftArmEntity, leftComp);

    AnimationComponent rightComp;
    rightComp.baseLayer = rightLayer;
    rightComp.speed = 1.0f;
    animations.Add(rightArmEntity, rightComp);

    // Systems
    MovementSystem movementSystem(transforms, velocities, accelerations);
    CameraSystem cameraSystem(transforms, cameras);
    cameraSystem.SetActiveCamera(player);

    SystemManager systemManager;
    AnimationSystem animationSystem(animations, sprites, transforms);
    systemManager.RegisterSystem<AnimationSystem>(animationSystem);

    systemManager.RegisterSystem<MovementSystem>(movementSystem);
    systemManager.RegisterSystem<CameraSystem>(cameraSystem);
    systemManager.RegisterSystem<BoundrySystem>(transforms, boundaries, &window);
    EventBus eventBus;
    systemManager.RegisterSystem<CollisionSystem>(entityManager, transforms, colliders, eventBus);
    SpatialGrid<EntityID> spatialGrid;

    SurfaceBehaviorSystem sbh(transforms, velocities, surfaces, spatialGrid);
    sbh.SetDefaultVelocities();
    systemManager.RegisterSystem<SurfaceBehaviorSystem>(sbh);

    // Background
    Texture background;
    if (!background.LoadFromFile("../assets/background.jpeg", renderer.GetSDLRenderer())) {
        std::cerr << "Failed to load texture." << std::endl;
        return -1;
    }

    RenderSystem renderSystem(transforms, sprites, &renderer);
    // renderSystem.AddBackgroundLayer(&background, 0.0f);

    // Input
    InputManager input;
    input.Bind("Left", SDL_SCANCODE_LEFT);
    input.Bind("Right", SDL_SCANCODE_RIGHT);
    input.Bind("Down", SDL_SCANCODE_DOWN);
    input.Bind("Up", SDL_SCANCODE_UP);

    eventBus.Subscribe<CollisionEvent>([&](const CollisionEvent& e) {
        std::cout << "HIT!!!\n";
    });

    // Main loop
    const float dt = 1.0f / 60.0f;

    while (window.IsRunning()) {
        window.PollEvents();
        input.Update();

        if (auto* velocity = velocities.Get(player)) {
            if (input.IsActionHeld("Left"))  velocity->dx -= 1;
            if (input.IsActionHeld("Right")) velocity->dx += 1;
            if (input.IsActionHeld("Down"))  velocity->dy += 1;
            if (input.IsActionHeld("Up"))    velocity->dy -= 1;
        }

        renderer.SetDrawColor(255, 255, 255, 255);
        systemManager.UpdateAll(1.0f);
        cameraSystem.ApplyToRenderSystem(renderSystem);
        aiSystem.Update(100.0f);
        animationSystem.Update(dt);

        renderer.Clear();
        renderSystem.Update(dt);

        SDL_Point camPos = renderSystem.GetCameraPosition();
        auto* body    = transforms.Get(stickmanEntity);
        auto* leftArm = transforms.Get(leftArmEntity);
        auto* rightArm= transforms.Get(rightArmEntity);

        if (body) {
            DrawStickman(&renderer, *body, leftArm, rightArm, camPos);
        }

        DrawGrid(renderer.GetSDLRenderer(), camPos, 800, 600, 64);

        SDL_Rect rect{ 500, 300, 200, 100 };
        renderer.SetDrawColor(200, 50, 50, 255);
        renderer.DrawRect(rect, true);          

        renderer.SetDrawColor(30, 30, 60, 255);
        renderer.DrawRect(rect, false);           

        renderer.SetDrawColor(0, 255, 0, 255);
        renderer.DrawLine(0, 0, 1200, 720);


        renderer.Present();

        SDL_Delay(1000 / 60);
    }

    texture.Unload();
    window.Shutdown();
    renderer.Shutdown();
    IMG_Quit();
    return 0;
}
