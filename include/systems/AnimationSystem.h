#pragma once

#include "core/ISystem.h"
#include "utils/AnimationUtils.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"     
#include "components/TransformComponent.h"
#include "components/SpriteComponent.h"

#include <cmath>
#include <algorithm>

class AnimationSystem : public ISystem {
public:
    AnimationSystem(ComponentStorage<AnimationComponent>& animations,
                    ComponentStorage<SpriteComponent>& sprites,
                    ComponentStorage<TransformComponent>& transforms);

    void Update(float deltaTime) override;

private:
    ComponentStorage<AnimationComponent>& m_animations;
    ComponentStorage<SpriteComponent>& m_sprites;
    ComponentStorage<TransformComponent>& m_transforms;

    // Helpers 
    void AdvanceLayer(AnimationLayer& layer, float dt);
    void ApplyLayer(EntityID entity, const AnimationLayer& layer);
    void UpdateStateMachine(AnimationComponent& anim, float dt);
    void ApplyStateMachine(AnimationComponent& anim, EntityID entity);

    // Interpolate and blending
    AnimationTransform Interpolate(const BoneAnimation& ba, float time, float duration, bool loop);
    TransformComponent Blend(const TransformComponent& base, const AnimationTransform& anim, float w);

    int GetFrameIndex(const FrameAnimationClip& clip, float time) const;
    float TotalDuration(const FrameAnimationClip& clip) const;

    // Lerp
    VectorFloat Lerp(const VectorFloat& a, const VectorFloat& b, float t) const {
        return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
    }
    float Lerp(float a, float b, float t) const { return a + (b - a) * t; }
};
