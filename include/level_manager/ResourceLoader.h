#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "utils/CollisionLayer.h"
#include "utils/SurfaceTypes.h"

#include "AI/AIController.h"
#include "AI/AIBehavior.h"

using json = nlohmann::json;

class Renderer;
class AssetManager;
class EntityCreationSystem;

template<typename T> class ComponentStorage;

class TransformComponent;
class SpriteComponent;
class PhysicsComponent;
class ColliderComponent;
class CameraComponent;
class SurfaceComponent;
class VelocityComponent;
class AccelerationComponent;
class BoundryComponent;

struct FrameAnimationClip;
struct SkeletalAnimationClip;
struct AnimationLayer;
struct AnimationComponent;
class EntityManager;
class EventBus;
class AISystem;

class ResourceLoader {
public:
    ResourceLoader(Renderer* renderer,
                   EntityCreationSystem* ecs,
                   AssetManager* assets,
                   EntityManager* em,
                   EventBus* eventBus,
                   AISystem* ai,
                   ComponentStorage<TransformComponent>* transforms,
                   ComponentStorage<BoundryComponent>* boundaries,
                   ComponentStorage<SpriteComponent>* sprites,
                   ComponentStorage<PhysicsComponent>* physics,
                   ComponentStorage<ColliderComponent>* colliders,
                   ComponentStorage<CameraComponent>* cameras,
                   ComponentStorage<SurfaceComponent>* surfaces,
                   ComponentStorage<VelocityComponent>* velocities,
                   ComponentStorage<AccelerationComponent>* accelerations,
                   ComponentStorage<AnimationComponent>* animations);

    bool LoadScene(const std::string& path);
    void UnloadScene();

private:
    Renderer* m_renderer;
    EntityCreationSystem* m_ecs;
    AssetManager* m_assets;
    EntityManager* m_em;
    EventBus* m_eventBus;
    AISystem* m_aiSystem;

    ComponentStorage<TransformComponent>* m_transforms;
    ComponentStorage<BoundryComponent>* m_boundaries;
    ComponentStorage<SpriteComponent>* m_sprites;
    ComponentStorage<PhysicsComponent>* m_physics;
    ComponentStorage<ColliderComponent>* m_colliders;
    ComponentStorage<CameraComponent>* m_cameras;
    ComponentStorage<SurfaceComponent>* m_surfaces;
    ComponentStorage<VelocityComponent>* m_velocities;
    ComponentStorage<AccelerationComponent>* m_accelerations;
    ComponentStorage<AnimationComponent>* m_animations;

    json m_prefabs;

    void LoadAssets(const json& j);
    void LoadPrefabs(const json& j);
    void LoadEntities(const json& j);

    // Parsers
    TransformComponent ParseTransform(const json& j);
    BoundryComponent ParseBoundry(const json& j);
    SpriteComponent ParseSprite(const json& j);
    PhysicsComponent ParsePhysics(const json& j);
    ColliderComponent ParseCollider(const json& j);
    CameraComponent ParseCamera(const json& j);
    SurfaceComponent ParseSurface(const json& j);
    VelocityComponent ParseVelocity(const json& j);
    AccelerationComponent ParseAcceleration(const json& j);
    AIController* ParseAIController(const json& j, EntityID id);

    FrameAnimationClip* ParseFrameClip(const json& j);
    SkeletalAnimationClip* ParseSkeletalClip(const json& j);
    AnimationLayer ParseAnimationLayer(const json& j);
    AnimationComponent ParseAnimation(const json& j);

    // helpers
    CollisionLayer StringToLayer(const std::string& s);
    SurfaceType StringToSurfaceType(const std::string& s);
    
    // AI
    std::unique_ptr<AIBehavior> CreateBehavior(const std::string& name);

};
