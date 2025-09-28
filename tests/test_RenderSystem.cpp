#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include "systems/RenderSystem.h"
#include "components/TransformComponent.h"
#include "scene/SpriteComponent.h"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"
#include "core/Entity.h"
#include "core/ComponentStorage.h"

// Dummy Texture that returns a valid SDL_Texture* for testing
class MockTexture : public Texture {
public:
    explicit MockTexture(SDL_Texture* tex) : m_texture(tex) {}
    SDL_Texture* GetSDLTexture() const override { return m_texture; }

private:
    SDL_Texture* m_texture;
};

// Mock Renderer that counts draw calls and captures draw parameters
class TestRenderer : public IRenderer {
public:
    int drawCalls = 0;
    SDL_Texture* lastTexture = nullptr;
    SDL_Rect lastDstRect = {};

    void DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect) override {
        drawCalls++;
        lastTexture = texture;
        if (dstRect) {
            lastDstRect = *dstRect;
        }
    }
};

TEST(RenderSystemTest, RendersEntityWithValidComponents) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<SpriteComponent> sprites;
    TestRenderer renderer;

    SDL_Texture* dummySDLTexture = reinterpret_cast<SDL_Texture*>(0x1); // dummy texture object
    MockTexture texture(dummySDLTexture);

    Entity entity(1);
    transforms.Add(entity.GetID(), {100, 200, 64, 64});
    sprites.Add(entity.GetID(), {&texture, 64, 64});

    RenderSystem system(transforms, sprites, &renderer);
    system.Update(0.016f); // simulate frame update

    EXPECT_EQ(renderer.drawCalls, 1);
    EXPECT_EQ(renderer.lastDstRect.x, 100);
    EXPECT_EQ(renderer.lastDstRect.y, 200);
    EXPECT_EQ(renderer.lastDstRect.w, 64);
    EXPECT_EQ(renderer.lastDstRect.h, 64);
}

TEST(RenderSystemTest, SkipsEntityWithoutTransform) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<SpriteComponent> sprites;
    TestRenderer renderer;

    SDL_Texture* dummySDLTexture = reinterpret_cast<SDL_Texture*>(0x1);
    MockTexture texture(dummySDLTexture);

    Entity entity(2);
    // No transform added
    sprites.Add(entity.GetID(), {&texture, 64, 64});

    RenderSystem system(transforms, sprites, &renderer);
    system.Update(0.016f);

    EXPECT_EQ(renderer.drawCalls, 0);
}

TEST(RenderSystemTest, SkipsEntityWithoutTexture) {
    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<SpriteComponent> sprites;
    TestRenderer renderer;

    Entity entity(3);
    transforms.Add(entity.GetID(), {50, 50, 32, 32});
    sprites.Add(entity.GetID(), {nullptr, 32, 32}); // no texture

    RenderSystem system(transforms, sprites, &renderer);
    system.Update(0.016f);

    EXPECT_EQ(renderer.drawCalls, 0);
}
