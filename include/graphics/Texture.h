#pragma once

#include <SDL2/SDL.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    // Load texture from file and unload
    bool LoadFromFile(const std::string& path, SDL_Renderer* renderer);
    void Unload();

    // Getter and setter
    virtual SDL_Texture* GetSDLTexture() const;
    void SetSDLTexture(SDL_Texture* tex);

private:
    SDL_Texture* m_texture;
};
