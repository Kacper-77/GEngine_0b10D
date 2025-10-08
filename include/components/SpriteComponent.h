#pragma once

#include "SDL.h"
#include "../graphics/Texture.h"

struct SpriteComponent {
    Texture* texture = nullptr;
    int width = 0;
    int height = 0;

    void SetTexture(Texture* tex) {
        texture = tex;
        if (texture) {
            SDL_QueryTexture(texture->GetSDLTexture(), NULL, NULL, &width, &height);
        }
    }
};
