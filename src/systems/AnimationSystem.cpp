#include "systems/AnimationSystem.h"
#include "components/SpriteComponent.h" // jeśli używasz poklatkowych animacji

AnimationSystem::AnimationSystem(ComponentStorage<AnimationComponent>& animations,
                                 ComponentStorage<SpriteComponent>& sprites,
                                 ComponentStorage<TransformComponent>& transforms)
    : m_animations{animations}, m_sprites{sprites}, m_transforms{transforms} {}

void AnimationSystem::Update(float deltaTime) {
    for (auto& [entity, anim] : m_animations.GetAll()) {
        if (anim.stateMachine.has_value()) {
            UpdateStateMachine(anim, deltaTime);
            ApplyStateMachine(anim, entity);
        } else {
            AdvanceLayer(anim.baseLayer, deltaTime * anim.speed);
            ApplyLayer(entity, anim.baseLayer);
        }
    }
}

void AnimationSystem::AdvanceLayer(AnimationLayer& layer, float dt) {
    if (!layer.clip) return;
    layer.time += dt;

    if (layer.type == AnimationType::FRAME_BASED) {
        const auto* clip = static_cast<const FrameAnimationClip*>(layer.clip);
        float total = TotalDuration(*clip);
        if (layer.loop && total > 0.0f) {
            layer.time = std::fmod(layer.time, total);
        }
    } else {
        const auto* clip = static_cast<const SkeletalAnimationClip*>(layer.clip);
        float total = clip->duration;
        if (layer.loop && total > 0.0f) {
            layer.time = std::fmod(layer.time, total);
        }
    }
}

void AnimationSystem::ApplyLayer(EntityID entity, const AnimationLayer& layer) {
    if (!layer.clip) return;

    if (layer.type == AnimationType::FRAME_BASED) {
        const auto* clip = static_cast<const FrameAnimationClip*>(layer.clip);
        int frameIndex = GetFrameIndex(*clip, layer.time);
        Texture* tex = clip->frames[frameIndex].texture;

        if (auto* sc = m_sprites.Get(entity)) {
            sc->SetTexture(tex);
        }
    } else {
        const auto* clip = static_cast<const SkeletalAnimationClip*>(layer.clip);
        for (const BoneAnimation& ba : clip->bones) {
            AnimationTransform animT = Interpolate(ba, layer.time, clip->duration, clip->loop);

            if (auto* worldT = m_transforms.Get(ba.boneID)) {
                TransformComponent blended = Blend(*worldT, animT, layer.weight);
                *worldT = blended;
            }
        }
    }
}
AnimationTransform AnimationSystem::Interpolate(const BoneAnimation& ba, float time, float duration, bool loop) {
    AnimationTransform out{};
    if (ba.frames.empty()) return out;

    float t = time;
    if (loop && duration > 0.0f) {
        t = std::fmod(t, duration);
        if (t < 0) t += duration;
    }

    const Keyframe* prev = &ba.frames.front();
    const Keyframe* next = &ba.frames.back();

    for (size_t i = 1; i < ba.frames.size(); ++i) {
        if (ba.frames[i].time >= t) {
            next = &ba.frames[i];
            prev = &ba.frames[i - 1];
            break;
        }
    }

    float dt = std::max(0.0001f, next->time - prev->time);
    float alpha = std::clamp((t - prev->time) / dt, 0.0f, 1.0f);

    out.position    = Lerp(prev->transform.position,    next->transform.position,    alpha);
    out.scale       = Lerp(prev->transform.scale,       next->transform.scale,       alpha);
    out.rotationDeg = Lerp(prev->transform.rotationDeg, next->transform.rotationDeg, alpha);
    return out;
}

TransformComponent AnimationSystem::Blend(const TransformComponent& base, const AnimationTransform& anim, float w) {
    TransformComponent out = base;
    out.position    = Lerp(base.position,    anim.position,    w);
    out.scale       = Lerp(base.scale,       anim.scale,       w);
    out.rotationDeg = Lerp(base.rotationDeg, anim.rotationDeg, w);
    return out;
}

int AnimationSystem::GetFrameIndex(const FrameAnimationClip& clip, float time) const {
    float t = time;
    float acc = 0.0f;
    for (int i = 0; i < (int)clip.frames.size(); ++i) {
        acc += clip.frames[i].duration;
        if (t < acc) return i;
    }
    return (int)clip.frames.size() - 1;
}

float AnimationSystem::TotalDuration(const FrameAnimationClip& clip) const {
    float sum = 0.0f;
    for (auto& f : clip.frames) sum += f.duration;
    return sum;
}

void AnimationSystem::UpdateStateMachine(AnimationComponent& anim, float dt) {
    auto& sm = *anim.stateMachine;
    auto& layers = sm.states[sm.currentState];

    for (auto& layer : layers) AdvanceLayer(layer, dt * anim.speed);

    for (auto& tr : sm.transitions) {
        if (tr.from == sm.currentState && tr.condition && tr.condition()) {
            sm.currentState = tr.to;
            break;
        }
    }
}

void AnimationSystem::ApplyStateMachine(AnimationComponent& anim, EntityID entity) {
    auto& sm = *anim.stateMachine;
    auto it = sm.states.find(sm.currentState);
    if (it == sm.states.end()) return;

    for (const auto& layer : it->second) {
        ApplyLayer(entity, layer);
    }
}
