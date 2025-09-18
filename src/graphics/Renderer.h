#pragma once

#include <SDL2/SDL.h>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Init(SDL_Window* window);
    void Shutdown();

    void Clear();
    void Present();
    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    void DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect);

    SDL_Renderer* GetSDLRenderer() const;

private:
    SDL_Renderer* m_renderer;
};
