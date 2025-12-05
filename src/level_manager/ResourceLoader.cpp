#include "level_manager/ResourceLoader.h"
#include "level_manager/AssetManager.h"
#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"
#include "core/ComponentStorage.h"

#include "components/TransformComponent.h"
#include "components/SpriteComponent.h"
#include "components/PhysicsComponent.h"
#include "components/ColliderComponent.h"
#include "components/CameraComponent.h"
#include "components/SurfaceComponent.h"
#include "components/VelocityComponent.h"
#include "components/AccelerationComponent.h"
#include "components/BoundryComponent.h"
#include "utils/AnimationUtils.h"

#include "graphics/Renderer.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

ResourceLoader::ResourceLoader(Renderer* renderer,
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
                               ComponentStorage<AnimationComponent>* animations)
    : m_renderer{renderer}, m_ecs{ecs}, m_assets{assets}, m_em{em},
      m_transforms{transforms}, m_boundaries{boundaries}, m_sprites{sprites}, m_physics{physics},
      m_colliders{colliders}, m_cameras{cameras}, m_surfaces{surfaces},
      m_velocities{velocities}, m_accelerations{accelerations}, m_animations{animations} {}

bool ResourceLoader::LoadScene(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ResourceLoader: Failed to open " << path << "\n";
        return false;
    }

    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        std::cerr << "ResourceLoader: JSON parse error: " << e.what() << "\n";
        return false;
    }

    if (j.contains("assets")) {
        LoadAssets(j["assets"]);
    }

    if (j.contains("prefabs")) {
        LoadPrefabs(j["prefabs"]);
    }

    if (j.contains("entities")) {
        LoadEntities(j["entities"]);
    }

    return true;
}

void ResourceLoader::UnloadScene() {
    if (m_assets) m_assets->UnloadAll();
}

void ResourceLoader::LoadAssets(const json& j) {
    if (!j.contains("textures")) return;
    for (auto& [key, path] : j["textures"].items()) {
        if (!m_assets->LoadTexture(key, path.get<std::string>())) {
            std::cerr << "ResourceLoader: Failed to load texture " << key
                      << " -> " << path << "\n";
        }
    }
}

void ResourceLoader::LoadPrefabs(const json& j) {
    m_prefabs = j;
}

void ResourceLoader::LoadEntities(const json& j) {
    for (auto& e : j) {
        EntityID id{};

        // Prefab-based entity
        if (e.contains("prefab")) {
            std::string prefabName = e["prefab"].get<std::string>();
            if (!m_prefabs.contains(prefabName)) {
                std::cerr << "ResourceLoader: prefab not found: "
                          << prefabName << "\n";
                continue;
            }

            json prefab = m_prefabs[prefabName];

            // Shallow-merge components
            json components = prefab.value("components", json::object());
            if (e.contains("components")) {
                for (auto& [k, v] : e["components"].items()) {
                    components[k] = v;
                }
            }

            id = m_ecs->CreateEntity();

            // Meta: tag / info / respawn / drops
            if (e.contains("tag")) {
                m_em->AddTag(id, e["tag"].get<std::string>());
            }
            if (e.contains("info")) {
                for (auto& [key, val] : e["info"].items()) {
                    m_em->AddInfo(id, key, val.get<std::string>());
                }
            }
            if (e.contains("respawn")) {
                RespawnInfo r;
                r.position.x   = e["respawn"].value("x", 0.0f);
                r.position.y   = e["respawn"].value("y", 0.0f);
                r.respawnDelay = e["respawn"].value("delay", 0.0f);
                m_em->SetRespawnPoint(id, r);
            }
            if (e.contains("drops")) {
                for (auto& d : e["drops"]) {
                    DropInfo drop;
                    drop.itemID     = d.value("itemID", 0u);
                    std::string rar = d.value("rarity", "COMMON");
                    if      (rar == "RARE")      drop.rarity = Rarity::RARE;
                    else if (rar == "EPIC")      drop.rarity = Rarity::EPIC;
                    else if (rar == "LEGENDARY") drop.rarity = Rarity::LEGENDARY;
                    else if (rar == "CUSTOM")    drop.rarity = Rarity::CUSTOM;
                    else                         drop.rarity = Rarity::COMMON;

                    drop.quantity   = d.value("quantity", static_cast<uint8_t>(1));
                    drop.dropChance = d.value("chance", 1.0f); // alias "chance" w JSON

                    m_em->AddDrop(id, drop);
                }
            }

            if (components.contains("Transform") && m_transforms) {
                m_transforms->Add(id, ParseTransform(components["Transform"]));
            }
            if (components.contains("Sprite") && m_sprites) {
                m_sprites->Add(id, ParseSprite(components["Sprite"]));
            }
            if (components.contains("Physics") && m_physics) {
                m_physics->Add(id, ParsePhysics(components["Physics"]));
            }
            if (components.contains("Collider") && m_colliders) {
                m_colliders->Add(id, ParseCollider(components["Collider"]));
            }
            if (components.contains("Camera") && m_cameras) {
                m_cameras->Add(id, ParseCamera(components["Camera"]));
            }
            if (components.contains("Surface") && m_surfaces) {
                m_surfaces->Add(id, ParseSurface(components["Surface"]));
            }
            if (components.contains("Velocity") && m_velocities) {
                m_velocities->Add(id, ParseVelocity(components["Velocity"]));
            }
            if (components.contains("Acceleration") && m_accelerations) {
                m_accelerations->Add(id, ParseAcceleration(components["Acceleration"]));
            }
            if (components.contains("Animation") && m_animations) {
                m_animations->Add(id, ParseAnimation(components["Animation"]));
            }

            continue;
        }

        // Normal entity
        id = m_ecs->CreateEntity();

        if (e.contains("tag")) {
            m_em->AddTag(id, e["tag"].get<std::string>());
        }
        if (e.contains("info")) {
            for (auto& [key, val] : e["info"].items()) {
                m_em->AddInfo(id, key, val.get<std::string>());
            }
        }
        if (e.contains("respawn")) {
            RespawnInfo r;
            r.position.x   = e["respawn"].value("x", 0.0f);
            r.position.y   = e["respawn"].value("y", 0.0f);
            r.respawnDelay = e["respawn"].value("delay", 0.0f);
            m_em->SetRespawnPoint(id, r);
        }
        if (e.contains("drops")) {
            for (auto& d : e["drops"]) {
                DropInfo drop;
                drop.itemID     = d.value("itemID", 0u);
                std::string rar = d.value("rarity", "COMMON");
                if      (rar == "RARE")      drop.rarity = Rarity::RARE;
                else if (rar == "EPIC")      drop.rarity = Rarity::EPIC;
                else if (rar == "LEGENDARY") drop.rarity = Rarity::LEGENDARY;
                else if (rar == "CUSTOM")    drop.rarity = Rarity::CUSTOM;
                else                         drop.rarity = Rarity::COMMON;

                drop.quantity   = d.value("quantity", static_cast<uint8_t>(1));
                drop.dropChance = d.value("chance", 1.0f);

                m_em->AddDrop(id, drop);
            }
        }

        auto& comps = e["components"];
        if (comps.contains("Transform") && m_transforms) {
            m_transforms->Add(id, ParseTransform(comps["Transform"]));
        }
        if (comps.contains("Boundry"))
            m_boundaries->Add(id, ParseBoundry(comps["Boundry"]));
        if (comps.contains("Sprite") && m_sprites) {
            m_sprites->Add(id, ParseSprite(comps["Sprite"]));
        }
        if (comps.contains("Physics") && m_physics) {
            m_physics->Add(id, ParsePhysics(comps["Physics"]));
        }
        if (comps.contains("Collider") && m_colliders) {
            m_colliders->Add(id, ParseCollider(comps["Collider"]));
        }
        if (comps.contains("Camera") && m_cameras) {
            m_cameras->Add(id, ParseCamera(comps["Camera"]));
        }
        if (comps.contains("Surface") && m_surfaces) {
            m_surfaces->Add(id, ParseSurface(comps["Surface"]));
        }
        if (comps.contains("Velocity") && m_velocities) {
            m_velocities->Add(id, ParseVelocity(comps["Velocity"]));
        }
        if (comps.contains("Acceleration") && m_accelerations) {
            m_accelerations->Add(id, ParseAcceleration(comps["Acceleration"]));
        }
        if (comps.contains("Animation") && m_animations) {
            m_animations->Add(id, ParseAnimation(comps["Animation"]));
        }
    }
}

// Parsers 

TransformComponent ResourceLoader::ParseTransform(const json& j) {
    TransformComponent t;
    t.position.x  = j.value("x", 0.0f);
    t.position.y  = j.value("y", 0.0f);
    t.rotationDeg = j.value("rotation", 0.0f);
    t.scale.x     = j.value("w", 1.0f);
    t.scale.y     = j.value("h", 1.0f);
    return t;
}

BoundryComponent ResourceLoader::ParseBoundry(const json& j) {
    BoundryComponent b;
    b.blockLeft   = j.value("left", true);
    b.blockRight  = j.value("right", true);
    b.blockTop    = j.value("top", true);
    b.blockBottom = j.value("bottom", true);
    return b;
}

SpriteComponent ResourceLoader::ParseSprite(const json& j) {
    SpriteComponent s;
    std::string texKey = j.value("texture", "");
    s.texture = m_assets->GetTexture(texKey);
    s.width   = j.value("w", 0);
    s.height  = j.value("h", 0);
    return s;
}

PhysicsComponent ResourceLoader::ParsePhysics(const json& j) {
    PhysicsComponent p;
    p.SetMass(j.value("mass", 1.0f));
    p.gravityScale   = j.value("gravityScale", 1.0f);
    p.linearDamping  = j.value("linearDamping", 0.0f);
    p.maxSpeed       = j.value("maxSpeed", 1000.0f);
    return p;
}

ColliderComponent ResourceLoader::ParseCollider(const json& j) {
    ColliderComponent c;
    c.width  = j.value("w", 0);
    c.height = j.value("h", 0);
    c.layer  = StringToLayer(j.value("layer", "None"));
    c.mask   = StringToLayer(j.value("mask", "All"));
    return c;
}

CameraComponent ResourceLoader::ParseCamera(const json& j) {
    CameraComponent cam;

    // Basic
    cam.isActive = j.value("active", true);

    cam.viewportSize.x = j.value("viewportW", 800);
    cam.viewportSize.y = j.value("viewportH", 600);

    cam.offset.x = j.value("offsetX", 0);
    cam.offset.y = j.value("offsetY", 0);

    cam.zoom = j.value("zoom", 1.0f);
    cam.rotationDegrees = j.value("rotation", 0.0f);

    // Position
    cam.position.x = j.value("posX", 0);
    cam.position.y = j.value("posY", 0);

    // Target entity (by tag)
    if (j.contains("target")) {
        std::string tag = j["target"].get<std::string>();
        const auto& group = m_em->GetGroup(tag);
        if (!group.empty()) {
            cam.target = *group.begin();
        } else {
            std::cerr << "Camera target tag not found: " << tag << "\n";
            cam.target = INVALID_ENTITY;
        }
    }

    // Mode
    std::string modeStr = j.value("mode", "Static");
    if      (modeStr == "Static")       cam.mode = CameraMode::Static;
    else if (modeStr == "Follow")       cam.mode = CameraMode::Follow;
    else if (modeStr == "SmoothFollow") cam.mode = CameraMode::SmoothFollow;
    else if (modeStr == "Scripted")     cam.mode = CameraMode::Scripted;
    else if (modeStr == "RoomBased")    cam.mode = CameraMode::RoomBased;
    else if (modeStr == "Shake")        cam.mode = CameraMode::Shake;
    else {
        std::cerr << "Unknown camera mode: " << modeStr << " (default Static)\n";
        cam.mode = CameraMode::Static;
    }

    // Smoothing
    cam.smoothing = j.value("smoothing", 8);

    // Bounds
    cam.useBounds = j.value("useBounds", false);
    if (j.contains("bounds")) {
        cam.bounds.x = j["bounds"].value("x", 0);
        cam.bounds.y = j["bounds"].value("y", 0);
        cam.bounds.w = j["bounds"].value("w", 0);
        cam.bounds.h = j["bounds"].value("h", 0);
    }

    // --- Manual control ---
    cam.manualControl = j.value("manual", false);

    // Effects
    cam.shakeIntensity = j.value("shakeIntensity", 0);
    cam.shakeDuration  = j.value("shakeDuration", 0.0f);
    cam.fadeAlpha      = j.value("fadeAlpha", 0);

    // Scripted target (for Scripted mode)
    cam.scriptTarget.x = j.value("scriptX", 0);
    cam.scriptTarget.y = j.value("scriptY", 0);

    return cam;
}


SurfaceComponent ResourceLoader::ParseSurface(const json& j) {
    SurfaceComponent s;
    s.x           = j.value("x", 0.0f);
    s.y           = j.value("y", 0.0f);
    s.width       = j.value("w", 0.0f);
    s.height      = j.value("h", 0.0f);
    s.surfaceType = StringToSurfaceType(j.value("type", "CUSTOM"));
    return s;
}

VelocityComponent ResourceLoader::ParseVelocity(const json& j) {
    VelocityComponent v;
    v.dx = j.value("dx", 0.0f);
    v.dy = j.value("dy", 0.0f);
    return v;
}

AccelerationComponent ResourceLoader::ParseAcceleration(const json& j) {
    AccelerationComponent a;
    a.ax = j.value("ax", 0.0f);
    a.ay = j.value("ay", 0.0f);
    return a;
}

// Animations
FrameAnimationClip* ResourceLoader::ParseFrameClip(const json& j) {
    auto* clip = new FrameAnimationClip();
    clip->name = j.value("name", "unnamed");
    clip->loop = j.value("loop", true);

    for (auto& f : j["frames"]) {
        Frame frame;
        std::string texKey = f.value("texture", "");
        frame.texture      = m_assets->GetTexture(texKey);
        frame.duration     = f.value("duration", 0.1f);
        clip->frames.push_back(frame);
    }
    return clip;
}

SkeletalAnimationClip* ResourceLoader::ParseSkeletalClip(const json& j) {
    auto* clip = new SkeletalAnimationClip();
    clip->name     = j.value("name", "unnamed");
    clip->duration = j.value("duration", 1.0f);
    clip->loop     = j.value("loop", true);

    for (auto& b : j["bones"]) {
        BoneAnimation bone;

        std::string boneTag = b.value("boneID", "");
        const auto& group   = m_em->GetGroup(boneTag);
        if (!group.empty()) {
            bone.boneID = *group.begin();
        } else {
            bone.boneID = INVALID_ENTITY; 
            std::cerr << "ResourceLoader: Bone tag not found: " << boneTag << "\n";
        }

        for (auto& k : b["keyframes"]) {
            Keyframe key;
            key.time = k.value("time", 0.0f);

            key.transform.position.x = k["pos"].value("x", 0.0f);
            key.transform.position.y = k["pos"].value("y", 0.0f);
            key.transform.rotationDeg = k.value("rot", 0.0f);
            key.transform.scale.x = k["scale"].value("x", 1.0f);
            key.transform.scale.y = k["scale"].value("y", 1.0f);

            bone.frames.push_back(key);
        }

        clip->bones.push_back(bone);
    }

    return clip;
}

AnimationLayer ResourceLoader::ParseAnimationLayer(const json& j) {
    AnimationLayer layer;

    layer.weight = j.value("weight", 1.0f);
    layer.loop   = j.value("loop", true);

    std::string typeStr = j.value("type", "FRAME_BASED");
    layer.type = (typeStr == "SKELETAL") ? AnimationType::SKELETAL
                                         : AnimationType::FRAME_BASED;

    if (layer.type == AnimationType::FRAME_BASED) {
        layer.clip = ParseFrameClip(j["clip"]);
    } else {
        layer.clip = ParseSkeletalClip(j["clip"]);
    }

    if (j.contains("events")) {
        for (auto& e : j["events"]) {
            AnimationEvent ev;
            ev.time      = e.value("time", 0.0f);
            ev.eventName = e.value("name", "");
            layer.events.push_back(ev);
        }
    }

    return layer;
}

AnimationComponent ResourceLoader::ParseAnimation(const json& j) {
    AnimationComponent anim;

    anim.speed = j.value("speed", 1.0f);

    std::string type = j.value("type", "FRAME_BASED");
    anim.type = (type == "SKELETAL") ? AnimationType::SKELETAL
                                     : AnimationType::FRAME_BASED;

    anim.baseLayer = ParseAnimationLayer(j);

    if (j.contains("stateMachine")) {
        AnimationStateMachine sm;
        const json& smj = j["stateMachine"];

        sm.currentState = smj.value("initial", "");

        if (smj.contains("states")) {
            for (auto& [stateName, layers] : smj["states"].items()) {
                for (auto& layerJson : layers) {
                    sm.states[stateName].push_back(ParseAnimationLayer(layerJson));
                }
            }
        }

        anim.stateMachine = sm;
    }

    return anim;
}

// Helpers 

CollisionLayer ResourceLoader::StringToLayer(const std::string& s) {
    if (s == "None")        return CollisionLayer::None;
    if (s == "Player")      return CollisionLayer::Player;
    if (s == "Enemy")       return CollisionLayer::Enemy;
    if (s == "Projectile")  return CollisionLayer::Projectile;
    if (s == "Wall")        return CollisionLayer::Wall;
    if (s == "Pickup")      return CollisionLayer::Pickup;
    if (s == "Trigger")     return CollisionLayer::Trigger;
    if (s == "Sensor")      return CollisionLayer::Sensor;
    if (s == "Environment") return CollisionLayer::Environment;
    if (s == "All")         return CollisionLayer::All;

    std::cerr << "Unknown CollisionLayer: " << s << " (defaulting to None)\n";
    return CollisionLayer::None;
}

SurfaceType ResourceLoader::StringToSurfaceType(const std::string& s) {
    if (s == "ICE")    return SurfaceType::ICE;
    if (s == "SAND")   return SurfaceType::SAND;
    if (s == "GRASS")  return SurfaceType::GRASS;
    if (s == "LIQUID") return SurfaceType::LIQUID;
    if (s == "CUSTOM") return SurfaceType::CUSTOM;

    std::cerr << "Unknown SurfaceType: " << s << " (defaulting to CUSTOM)\n";
    return SurfaceType::CUSTOM;
}
