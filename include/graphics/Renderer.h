#pragma once

#include "core/IRenderer.h"

class Renderer : public IRenderer {
public:
    Renderer();
    ~Renderer();

    bool Init(SDL_Window* window);
    void Shutdown();

    void Clear();
    void Present();
    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    virtual void DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) override;

    SDL_Renderer* GetSDLRenderer() const;

private:
    SDL_Renderer* m_renderer;
};
