#pragma once

#include <SDL2/SDL.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    bool LoadFromFile(const std::string& path, SDL_Renderer* renderer);
    void Unload();

    SDL_Texture* GetSDLTexture() const;

private:
    SDL_Texture* m_texture;
};
