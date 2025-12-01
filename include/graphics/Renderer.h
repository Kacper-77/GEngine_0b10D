#pragma once

#include "core/IRenderer.h"
#include <SDL2/SDL.h>
#include <string>

class Renderer : public IRenderer {
public:
    Renderer();
    ~Renderer();

    // Initiation, shutdown
    bool Init(SDL_Window* window, bool vsync = true, bool accelerated = true);
    void Shutdown();

    // Clear current state
    void Clear();

    // Get current state 
    void Present();

    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    // IRenderer method
    void DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) override;

    // Extended drawing
    void DrawTextureEx(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect,
                       double angle, SDL_Point* center, SDL_RendererFlip flip);

    void DrawPoint(int x, int y);

    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawRect(const SDL_Rect& rect, bool filled = false);

    // Logical size / scaling
    void SetLogicalSize(int w, int h);
    void SetViewport(const SDL_Rect& rect);
    void ResetViewport();

    // Info
    SDL_Renderer* GetSDLRenderer() const;
    std::string GetRendererInfo() const;

private:
    SDL_Renderer* m_renderer;
};
