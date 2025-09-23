#include <gtest/gtest.h>
#include "core/SystemManager.h"

class MockSystem : public ISystem {
public:
    bool updated = false;
    float lastDelta = -1.0f;

    void Update(float dt) override {
        updated = true;
        lastDelta = dt;
    }
};

TEST(SystemManagerTest, UpdatesCorrectSystem) {
    SystemManager manager;
    manager.RegisterSystem<MockSystem>();

    manager.UpdateAll(0.5f);

    auto* system = manager.GetSystem<MockSystem>();
    ASSERT_NE(system, nullptr);
    EXPECT_TRUE(system->updated);
    EXPECT_FLOAT_EQ(system->lastDelta, 0.5f);
}
