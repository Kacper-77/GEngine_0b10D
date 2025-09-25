#include "graphics/Texture.h"
#include <SDL_image.h>
#include <iostream>

Texture::Texture()
    : m_texture(nullptr) {}

Texture::~Texture() {
    Unload();
}

bool Texture::LoadFromFile(const std::string& path, SDL_Renderer* renderer) {
    Unload();

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!m_texture) {
        std::cerr << "Failed to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Texture::Unload() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

SDL_Texture* Texture::GetSDLTexture() const {
    return m_texture;
}

void Texture::SetSDLTexture(SDL_Texture* tex) {
    m_texture = tex;
}
