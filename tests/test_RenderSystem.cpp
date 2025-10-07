#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include "systems/RenderSystem.h"
#include "components/TransformComponent.h"
#include "scene/SpriteComponent.h"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"
#include "core/ComponentStorage.h"
#include "core/EntityManager.h"
#include "systems/EntityCreationSystem.h"

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

class RenderSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    EntityCreationSystem creationSystem{&entityManager};

    ComponentStorage<TransformComponent> transforms;
    ComponentStorage<SpriteComponent> sprites;
    TestRenderer renderer;

    SDL_Texture* dummySDLTexture = reinterpret_cast<SDL_Texture*>(0x1);
    MockTexture texture{dummySDLTexture};

    void SetUp() override {
        creationSystem.RegisterStorage(&transforms);
        creationSystem.RegisterStorage(&sprites);

        entityManager.RegisterComponentStorage(&transforms);
        entityManager.RegisterComponentStorage(&sprites);
    }
};

TEST_F(RenderSystemTest, RendersEntityWithValidComponents) {
    EntityID entity = creationSystem.CreateEntityWith(
        TransformComponent{100, 200, 64, 64},
        SpriteComponent{&texture, 64, 64}
    );

    RenderSystem system(transforms, sprites, &renderer);
    system.Update(0.016f);

    EXPECT_EQ(renderer.drawCalls, 1);
    EXPECT_EQ(renderer.lastDstRect.x, 100);
    EXPECT_EQ(renderer.lastDstRect.y, 200);
    EXPECT_EQ(renderer.lastDstRect.w, 64);
    EXPECT_EQ(renderer.lastDstRect.h, 64);
}

TEST_F(RenderSystemTest, SkipsEntityWithoutTransform) {
    EntityID entity = creationSystem.CreateEntityWith(
        SpriteComponent{&texture, 64, 64}
    );

    RenderSystem system(transforms, sprites, &renderer);
    system.Update(0.016f);

    EXPECT_EQ(renderer.drawCalls, 0);
}

TEST_F(RenderSystemTest, SkipsEntityWithoutTexture) {
    EntityID entity = creationSystem.CreateEntityWith(
        TransformComponent{50, 50, 32, 32},
        SpriteComponent{nullptr, 32, 32}
    );

    RenderSystem system(transforms, sprites, &renderer);
    system.Update(0.016f);

    EXPECT_EQ(renderer.drawCalls, 0);
}
