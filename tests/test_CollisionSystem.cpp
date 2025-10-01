#include "systems/CollisionSystem.h"
#include "components/ColliderComponent.h"
#include "core/EntityManager.h"
#include "core/ComponentStorage.h"
#include "event/core/EventManager.h"
#include "event/core/EventFactory.h"
#include "utils/EventResolver.h"

#include <gtest/gtest.h>
#include <vector>
#include <string>

class TestEventResolver : public EventResolver {
public:
    std::string ResolveEventType(const std::string& behaviorA,
                                 const std::string& behaviorB) const override {
        if (behaviorA == "slime" || behaviorB == "slime") return "Damage";
        if (behaviorA == "sticky" || behaviorB == "sticky") return "Stick";
        return "Nothing";
    }
};

class CollisionSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    ComponentStorage<ColliderComponent> colliders;
    EventManager eventManager;
    EventFactory eventFactory;
    TestEventResolver resolver;

    std::vector<std::string> emittedEvents;

    CollisionSystem system{entityManager, colliders, eventManager, eventFactory, resolver};

    void SetUp() override {
        eventManager.RegisterHandler("Damage", [&](const Event& e) {
            emittedEvents.push_back("Damage between " + std::to_string(e.source) + " and " + std::to_string(e.target));
        });

        eventManager.RegisterHandler("Stick", [&](const Event& e) {
            emittedEvents.push_back("Stick between " + std::to_string(e.source) + " and " + std::to_string(e.target));
        });
    }
};

TEST_F(CollisionSystemTest, EmitsDamageEventForSlimeCollision) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    entityManager.AddInfo(a, "behavior", "slime");
    entityManager.AddInfo(b, "behavior", "neutral");

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{5, 5, 10, 10}); // collision

    system.Update(0.0f);

    ASSERT_EQ(emittedEvents.size(), 1);
    ASSERT_EQ(emittedEvents[0], "Damage between " + std::to_string(a) + " and " + std::to_string(b));
}

TEST_F(CollisionSystemTest, EmitsStickEventForStickyCollision) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    entityManager.AddInfo(a, "behavior", "sticky");
    entityManager.AddInfo(b, "behavior", "neutral");

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{5, 5, 10, 10}); // collision

    system.Update(0.0f);

    ASSERT_EQ(emittedEvents.size(), 1);
    ASSERT_EQ(emittedEvents[0], "Stick between " + std::to_string(a) + " and " + std::to_string(b));
}

TEST_F(CollisionSystemTest, DoesNotEmitEventWhenNoCollisionOccurs) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    entityManager.AddInfo(a, "behavior", "slime");
    entityManager.AddInfo(b, "behavior", "sticky");

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{100, 100, 10, 10}); // no collision

    system.Update(0.0f);

    ASSERT_TRUE(emittedEvents.empty());
}

TEST_F(CollisionSystemTest, DoesNotEmitEventForUnknownBehavior) {
    EntityID a = entityManager.CreateEntity();
    EntityID b = entityManager.CreateEntity();

    entityManager.AddInfo(a, "behavior", "unknown");
    entityManager.AddInfo(b, "behavior", "neutral");

    colliders.Add(a, ColliderComponent{0, 0, 10, 10});
    colliders.Add(b, ColliderComponent{5, 5, 10, 10}); // collision

    system.Update(0.0f);

    ASSERT_TRUE(emittedEvents.empty());
}
