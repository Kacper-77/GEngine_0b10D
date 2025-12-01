#include "graphics/Renderer.h"
#include <iostream>

Renderer::Renderer()
    : m_renderer(nullptr) {}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Init(SDL_Window* window, bool vsync, bool accelerated) {
    Uint32 flags = 0;
    if (accelerated) flags |= SDL_RENDERER_ACCELERATED;
    else flags |= SDL_RENDERER_SOFTWARE;

    if (vsync) flags |= SDL_RENDERER_PRESENTVSYNC;

    m_renderer = SDL_CreateRenderer(window, -1, flags);

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

void Renderer::DrawTextureEx(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect,
                             double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(m_renderer, texture, srcRect, dstRect, angle, center, flip);
}

void Renderer::DrawPoint(int x, int y) {
    SDL_RenderDrawPoint(m_renderer, x, y);
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}

void Renderer::DrawRect(const SDL_Rect& rect, bool filled) {
    if (filled) SDL_RenderFillRect(m_renderer, &rect);
    else SDL_RenderDrawRect(m_renderer, &rect);
}

void Renderer::SetLogicalSize(int w, int h) {
    SDL_RenderSetLogicalSize(m_renderer, w, h);
}

void Renderer::SetViewport(const SDL_Rect& rect) {
    SDL_RenderSetViewport(m_renderer, &rect);
}

void Renderer::ResetViewport() {
    SDL_RenderSetViewport(m_renderer, nullptr);
}

SDL_Renderer* Renderer::GetSDLRenderer() const {
    return m_renderer;
}

std::string Renderer::GetRendererInfo() const {
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(m_renderer, &info) == 0) {
        return std::string("Renderer: ") + info.name;
    }
    return "Renderer info unavailable";
}
