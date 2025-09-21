#include "../include/scene/Sprite.h"

Sprite::Sprite()
    : m_texture(nullptr), m_x(0), m_y(0), m_width(0), m_height(0) {}

void Sprite::SetTexture(Texture* texture) {
    m_texture = texture;
}

void Sprite::SetPosition(int x, int y) {
    m_x = x;
    m_y = y;
}

void Sprite::SetSize(int width, int height) {
    m_width = width;
    m_height = height;
}

void Sprite::Draw(Renderer& renderer) {
    if (!m_texture) return;

    SDL_Rect dstRect = { m_x, m_y, m_width, m_height };
    renderer.DrawTexture(m_texture->GetSDLTexture(), nullptr, &dstRect);
}
