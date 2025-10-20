#include "graphics/Renderer.h"
#include <iostream>

Renderer::Renderer()
    : m_renderer(nullptr) {}

Renderer::~Renderer() {
    Shutdown();
}

// Init
bool Renderer::Init(SDL_Window* window) {
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Throw error if something went wrong
    if (!m_renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

// Shutdown renderer
void Renderer::Shutdown() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

// Clear the current rendering target with the drawing color
void Renderer::Clear() {
    SDL_RenderClear(m_renderer);
}

// Update the screen with any rendering performed since the previous call
void Renderer::Present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void Renderer::DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) {
    // Copy a portion of the texture to the current rendering target
    SDL_RenderCopy(m_renderer, texture, srcRect, dstRect);
}

SDL_Renderer* Renderer::GetSDLRenderer() const {
    return m_renderer;
}
