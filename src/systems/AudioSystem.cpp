#include "systems/AudioSystem.h"

#include <SDL2/SDL_mixer.h>
#include <cmath>


AudioSystem::AudioSystem(EntityManager* entityManager)
    : m_entityManager{entityManager} {

    m_layerVolumes[AudioLayer::UI] = MIX_MAX_VOLUME;
    m_layerVolumes[AudioLayer::Gameplay] = MIX_MAX_VOLUME;
    m_layerVolumes[AudioLayer::Ambient] = MIX_MAX_VOLUME;
    m_layerVolumes[AudioLayer::Music] = MIX_MAX_VOLUME;

    
    m_layerMuted[AudioLayer::UI] = false;
    m_layerMuted[AudioLayer::Gameplay] = false;
    m_layerMuted[AudioLayer::Ambient] = false;
    m_layerMuted[AudioLayer::Music] = false;
}

// Helper
static VectorFloat ToFloat(const Vector& v) {
    if (std::holds_alternative<VectorFloat>(v))
        return std::get<VectorFloat>(v);
    const auto& vi = std::get<VectorInt>(v);
    return VectorFloat{static_cast<float>(vi.x), static_cast<float>(vi.y)};
}

// PLAY
void AudioSystem::Play(EntityID id, SoundTag tag) {
    auto it = m_audioAndSounds.find(id);
    if (it != m_audioAndSounds.end()) {
        auto& sounds = it->second;
        auto soundIt = sounds.find(tag);
        if (soundIt != sounds.end()) {
            const AudioType& audio = soundIt->second;
            if (audio.type == AudioType::Type::Chunk)
                Mix_PlayChannel(-1, audio.chunk, 0);
            else
                Mix_PlayMusic(audio.music, -1);
        }
    }
}

void AudioSystem::Stop(EntityID id, SoundTag tag) {
    Mix_HaltChannel(-1);
}

// GLOBAL
void AudioSystem::PlayGlobal(SoundTag tag) {
    auto it = m_globalAudio.find(tag);
    if (it != m_globalAudio.end()) {
        const AudioType& audio = it->second;
        if (audio.type == AudioType::Type::Chunk)
            Mix_PlayChannel(-1, audio.chunk, 0);
        else
            Mix_PlayMusic(audio.music, -1);
    }
}

void AudioSystem::StopGlobal(SoundTag tag) {
    Mix_HaltChannel(-1);
    Mix_HaltMusic();
}

// REGISTRATION
void AudioSystem::RegisterSound(EntityID id, SoundTag tag, AudioType audio) {
    m_audioAndSounds[id][tag] = audio;
}

void AudioSystem::RegisterGlobalSound(SoundTag tag, AudioType audio) {
    m_globalAudio[tag] = audio;
}

void AudioSystem::RegisterLayeredSound(SoundTag tag, AudioType audio, AudioLayer layer) {
    m_layeredAudio[tag] = std::make_pair(audio, layer);
}

// SPATIAL AUDIOt
void AudioSystem::PlaySpatial(EntityID id, SoundTag tag, const Vector& listenerPos, int maxDistance) {
    auto posIt = m_entityPositions.find(id);
    if (posIt == m_entityPositions.end()) return;

    VectorFloat source = ToFloat(posIt->second);
    VectorFloat listener = ToFloat(listenerPos);
    float dx = source.x - listener.x;

    float pan = std::clamp(dx / static_cast<float>(maxDistance), -1.0f, 1.0f);
    int left = static_cast<int>((1.0f - pan) * MIX_MAX_VOLUME);
    int right = static_cast<int>((1.0f + pan) * MIX_MAX_VOLUME);

    auto& sounds = m_audioAndSounds[id];
    auto it = sounds.find(tag);
    if (it != sounds.end() && it->second.type == AudioType::Type::Chunk) {
        int channel = Mix_PlayChannel(-1, it->second.chunk, 0);
        Mix_SetPanning(channel, left, right);
    }
}

// === QUEUE ===
void AudioSystem::EnqueueSound(EntityID id, AudioType audio) {
    m_soundQueues[id].sounds.push(audio);
}

void AudioSystem::Update(float deltaTime) {
    for (auto& [id, queue] : m_soundQueues) {
        if (queue.isPlaying) {
            bool musicDone = Mix_PlayingMusic() == 0;
            bool chunkDone = Mix_Playing(-1) == 0;

            if (musicDone && chunkDone) {
                queue.isPlaying = false;
            } else {
                continue;
            }
        }

        if (!queue.sounds.empty()) {
            AudioType audio = queue.sounds.top();
            queue.sounds.pop();

            if (audio.type == AudioType::Type::Chunk) {
                int channel = Mix_PlayChannel(-1, audio.chunk, 0);
                if (channel != -1) {
                    queue.isPlaying = true;
                }
            } else {
                if (Mix_PlayMusic(audio.music, 0) == 0) {
                    queue.isPlaying = true;
                }
            }
        }
    }
}


// LAYER CONTROL
void AudioSystem::SetLayerVolume(AudioLayer layer, int volume) {
    m_layerVolumes[layer] = std::clamp(volume, 0, MIX_MAX_VOLUME);
}

void AudioSystem::MuteLayer(AudioLayer layer) {
    m_layerMuted[layer] = true;
}

void AudioSystem::UnmuteLayer(AudioLayer layer) {
    m_layerMuted[layer] = false;
}

// CLEANUP
void AudioSystem::CleanupUnused() {
    for (auto it = m_audioAndSounds.begin(); it != m_audioAndSounds.end();) {
        EntityID id = it->first;
        if (!m_entityManager->IsAlive(id)) {
            it = m_audioAndSounds.erase(it);
            m_entityPositions.erase(id);
            m_soundQueues.erase(id);
        } else {
            ++it;
        }
    }
}
