#pragma once

#include "utils/Vector.h"
#include "utils/EntityTypes.h"
#include "graphics/Texture.h"

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

enum class AnimationType { FRAME_BASED, SKELETAL };

struct AnimationTransform {
    VectorFloat position;
    float   rotationDeg;
    VectorFloat scale;
};

struct Frame {
    Texture* texture; 
    float duration;  // Frame duration
};

struct FrameAnimationClip {
    std::string name;
    std::vector<Frame> frames;
    bool loop = true;
};

struct Keyframe {
    float time;
    AnimationTransform transform;
};

struct BoneAnimation {
    EntityID boneID;
    std::vector<Keyframe> frames;  // Sorted in time
};

struct SkeletalAnimationClip {
    std::string name;
    float duration;
    std::vector<BoneAnimation> bones;
    bool loop = true;
};

struct AnimationEvent {
    float time;
    std::string eventName;
};

struct AnimationLayer {
    float weight = 1.0f;         // Layer blending
    const void* clip = nullptr;  // FrameAnimationClip* or SkeletalAnimationClip*
    AnimationType type = AnimationType::FRAME_BASED;
    float time = 0.0f;
    bool loop = true;
    std::vector<AnimationEvent> events;
};

struct AnimationStateMachine {
    std::string currentState;
    // Map: state -> layers list
    std::unordered_map<std::string, std::vector<AnimationLayer>> states;
    // predicate
    struct Transition {
        std::string from, to;
        float blendTime;
        std::function<bool()> condition;
    };
    std::vector<Transition> transitions;
};

struct AnimationComponent {
    AnimationType type = AnimationType::FRAME_BASED;
    // Main layer (no SM)
    AnimationLayer baseLayer;
    // Optional
    std::optional<AnimationStateMachine> stateMachine;
    // Global velocity
    float speed = 1.0f;
};
