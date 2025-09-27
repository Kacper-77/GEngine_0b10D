#pragma once

#include "SDL.h"

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) = 0;
};
