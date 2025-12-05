#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "utils/CollisionLayer.h"
#include "utils/SurfaceTypes.h"

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

class ResourceLoader {
public:
    ResourceLoader(Renderer* renderer,
                   EntityCreationSystem* ecs,
                   AssetManager* assets,
                   EntityManager* em,
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

    nlohmann::json m_prefabs;

    void LoadAssets(const nlohmann::json& j);
    void LoadPrefabs(const nlohmann::json& j);
    void LoadEntities(const nlohmann::json& j);

    // Parsers
    TransformComponent ParseTransform(const nlohmann::json& j);
    BoundryComponent ParseBoundry(const json& j);
    SpriteComponent ParseSprite(const nlohmann::json& j);
    PhysicsComponent ParsePhysics(const nlohmann::json& j);
    ColliderComponent ParseCollider(const nlohmann::json& j);
    CameraComponent ParseCamera(const nlohmann::json& j);
    SurfaceComponent ParseSurface(const nlohmann::json& j);
    VelocityComponent ParseVelocity(const nlohmann::json& j);
    AccelerationComponent ParseAcceleration(const nlohmann::json& j);

    FrameAnimationClip* ParseFrameClip(const nlohmann::json& j);
    SkeletalAnimationClip* ParseSkeletalClip(const nlohmann::json& j);
    AnimationLayer ParseAnimationLayer(const nlohmann::json& j);
    AnimationComponent ParseAnimation(const json& j);

    // helpers
    CollisionLayer StringToLayer(const std::string& s);
    SurfaceType StringToSurfaceType(const std::string& s);
};
