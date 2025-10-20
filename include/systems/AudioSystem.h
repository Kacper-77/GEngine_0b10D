#pragma once

#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include "core/ISystem.h"
#include "utils/AudioTypes.h"
#include "utils/EntityTypes.h"
#include "utils/Vector.h"
#include "core/EntityManager.h"

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
    AudioSystem(EntityManager* m_entityManager);

    // Play and stop entity sound
    void Play(EntityID id, SoundTag tag);
    void Stop(EntityID id, SoundTag tag);

    // Play and stop global sound
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
    void Update(float deltaTime) override;  // ISystem method

    void SetLayerVolume(AudioLayer layer, int volume);
    void MuteLayer(AudioLayer layer);
    void UnmuteLayer(AudioLayer layer);

    void CleanupUnused();

private:
    EntityManager* m_entityManager;
    std::unordered_map<EntityID, std::unordered_map<SoundTag, int>> m_entityChannels;
    std::unordered_map<EntityID, std::unordered_map<SoundTag, AudioType>> m_audioAndSounds;
    std::unordered_map<SoundTag, AudioType> m_globalAudio;

    std::unordered_map<SoundTag, std::pair<AudioType, AudioLayer>> m_layeredAudio;

    std::unordered_map<EntityID, Vector> m_entityPositions;

    struct SoundQueue {
        std::priority_queue<AudioType> sounds;
        bool isPlaying = false;
    };
    std::unordered_map<EntityID, SoundQueue> m_soundQueues;

    std::unordered_map<AudioLayer, int> m_layerVolumes;
    std::unordered_map<AudioLayer, bool> m_layerMuted;
};
