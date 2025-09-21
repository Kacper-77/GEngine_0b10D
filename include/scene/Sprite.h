#pragma once

#include "../graphics/Texture.h"
#include "../graphics/Renderer.h"

class Sprite {
public:
    Sprite();

    void SetTexture(Texture* texture);
    void SetPosition(int x, int y);
    void SetSize(int width, int height);

    void Draw(Renderer& renderer);

private:
    Texture* m_texture;
    int m_x, m_y;
    int m_width, m_height;
};
