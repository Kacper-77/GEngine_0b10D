#include <gtest/gtest.h>
#include "event/core/EventBus.h"
#include "event/custom_events/DamageEvent.h"
#include "event/custom_events/MovementEvent.h"
#include "event/custom_events/TeleportEvent.h"
#include "event/custom_events/PhysicsEvent.h"

// Test fixture for EventBus
class EventBusTest : public ::testing::Test {
protected:
    EventBus bus;
};

// Test that DamageEvent is correctly received and handled
TEST_F(EventBusTest, DamageEventIsHandledCorrectly) {
    bool handled = false;

    // Subscribe to DamageEvent and validate its content
    auto id = bus.Subscribe<DamageEvent>([&](const DamageEvent& e) {
        EXPECT_EQ(e.dest, 1);
        EXPECT_FLOAT_EQ(e.amount, 10.0f);
        handled = true;
    });

    // Publish the event
    DamageEvent e{1};
    e.amount = 10.0f;
    bus.Publish(e);
    bus.Dispatch();

    // Verify that the handler was triggered
    EXPECT_TRUE(handled);

    // Check listener count and unsubscribe
    EXPECT_EQ(bus.ListenerCount(typeid(DamageEvent)), 1);
    EXPECT_TRUE(bus.Unsubscribe(typeid(DamageEvent), id));
}

// Test that MovementEvent is correctly received and handled
TEST_F(EventBusTest, MovementEventIsHandledCorrectly) {
    bool handled = false;

    bus.Subscribe<MovementEvent>([&](const MovementEvent& e) {
        EXPECT_EQ(e.entity, 2);
        EXPECT_FLOAT_EQ(e.speedAmount, 2.5f);
        handled = true;
    });

    MovementEvent e{2};
    e.speedAmount = 2.5f;
    bus.Publish(e);
    bus.Dispatch();

    EXPECT_TRUE(handled);
}

// Test that TeleportEvent is correctly received and handled
TEST_F(EventBusTest, TeleportEventIsHandledCorrectly) {
    bool handled = false;

    bus.Subscribe<TeleportEvent>([&](const TeleportEvent& e) {
        EXPECT_EQ(e.entity, 3);
        EXPECT_EQ(e.x, 100);
        EXPECT_EQ(e.y, 200);
        handled = true;
    });

    TeleportEvent e{3, 100, 200};
    bus.Publish(e);
    bus.Dispatch();

    EXPECT_TRUE(handled);
}

// Test that PhysicsEvent is correctly received and handled
TEST_F(EventBusTest, PhysicsEventIsHandledCorrectly) {
    bool handled = false;

    bus.Subscribe<PhysicsEvent>([&](const PhysicsEvent& e) {
        EXPECT_EQ(e.entity, 4);
        EXPECT_FLOAT_EQ(e.gravity, -9.8f);
        EXPECT_FALSE(e.global);  // Ensure it's not a global event
        handled = true;
    });

    PhysicsEvent e{4};
    e.gravity = -9.8f;
    bus.Publish(e);
    bus.Dispatch();

    EXPECT_TRUE(handled);
}

// Test that PhysicsEvent is correctly received and handled but global :)
TEST_F(EventBusTest, PhysicsEventIsHandledCorrectlyGlobal) {
    bool handled = false;

    bus.Subscribe<PhysicsEvent>([&](const PhysicsEvent& e) {
        EXPECT_EQ(e.entity, 5);
        EXPECT_FLOAT_EQ(e.gravity, 6.66f);
        EXPECT_TRUE(e.global);  // Ensure it's global event
        handled = true;
    });

    PhysicsEvent e{5};
    e.gravity = 6.66f;
    e.global = true;
    bus.Publish(e);
    bus.Dispatch();

    EXPECT_TRUE(handled);
}