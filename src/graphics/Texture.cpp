#include "graphics/Texture.h"
#include <SDL_image.h>
#include <iostream>

Texture::Texture()
    : m_texture(nullptr) {}

Texture::~Texture() {
    Unload();
}

// Loading texture from file
bool Texture::LoadFromFile(const std::string& path, SDL_Renderer* renderer) {
    Unload();

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Throw error if something went wrong
    if (!m_texture) {
        std::cerr << "Failed to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

// Unload texture
void Texture::Unload() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

// Getter and setter
SDL_Texture* Texture::GetSDLTexture() const {
    return m_texture;
}

void Texture::SetSDLTexture(SDL_Texture* tex) {
    m_texture = tex;
}
