#pragma once

#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include "core/ISystem.h"
#include "utils/AudioTypes.h"
#include "utils/EntityTypes.h"
#include "utils/Vector.h"

using SoundTag = std::string;

using Vector = std::variant<VectorInt, VectorFloat>;

enum class AudioLayer {
    UI,
    Gameplay,
    Ambient,
    Music
};

class AudioSystem : public ISystem {
public:
    // Play Entity sound
    void Play(EntityID id, SoundTag tag);
    void Stop(EntityID id, SoundTag tag);

    // Play global sound
    void PlayGlobal(SoundTag tag);
    void StopGlobal(SoundTag tag);

    // Registration
    void RegisterSound(EntityID id, SoundTag tag, AudioType audio);
    void RegisterGlobalSound(SoundTag tag, AudioType audio);
    void RegisterLayeredSound(SoundTag tag, AudioType audio, AudioLayer layer);

    // Spatial audio - entity position
    void PlaySpatial(EntityID id, SoundTag tag, const Vector& listenerPos, int maxDistance = 500);

    // Queue
    void EnqueueSound(EntityID id, AudioType audio);
    void UpdateQueues(); // I need to implement this in global Update()

    void SetLayerVolume(AudioLayer layer, int volume);
    void MuteLayer(AudioLayer layer);
    void UnmuteLayer(AudioLayer layer);

    void CleanupUnused();

private:
    std::unordered_map<EntityID, std::unordered_map<SoundTag, AudioType>> m_audioAndSounds;
    std::unordered_map<SoundTag, AudioType> m_globalAudio;

    std::unordered_map<SoundTag, std::pair<AudioType, AudioLayer>> m_layeredAudio;

    std::unordered_map<EntityID, Vector> m_entityPositions;

    struct SoundQueue {
        std::queue<AudioType> sounds;
        bool isPlaying = false;
    };
    std::unordered_map<EntityID, SoundQueue> m_soundQueues;

    std::unordered_map<AudioLayer, int> m_layerVolumes;
    std::unordered_map<AudioLayer, bool> m_layerMuted;
};
