#include "../include/graphics/Renderer.h"
#include <iostream>

Renderer::Renderer()
    : m_renderer(nullptr) {}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Init(SDL_Window* window) {
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!m_renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Renderer::Shutdown() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

void Renderer::Clear() {
    SDL_RenderClear(m_renderer);
}

void Renderer::Present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void Renderer::DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) {
    SDL_RenderCopy(m_renderer, texture, srcRect, dstRect);
}

SDL_Renderer* Renderer::GetSDLRenderer() const {
    return m_renderer;
}
