#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <unordered_map>
#include "utils/AudioTypes.h"
#include "utils/EntityTypes.h"

using SoundTag = std::string;

class AudioSystem {
public:
    void Play(EntityID id, SoundTag tag);
    void Stop(EntityID id, SoundTag tag);
    void PlayGlobal(SoundTag tag);
    void RegisterSound(EntityID id, SoundTag tag, AudioType audio);
    void RegisterGlobalSound(SoundTag tag, AudioType audio);

private:
    std::unordered_map<EntityID, std::unordered_map<SoundTag, AudioType>> m_audioAndSounds;
    std::unordered_map<SoundTag, AudioType> m_globalAudio;
};