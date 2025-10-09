#pragma once

#include <SDL2/SDL_mixer.h>

struct AudioType {
    enum class Type { Chunk, Music };
    Type type;
    union {
        Mix_Chunk* chunk;
        Mix_Music* music;
    };
};