#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"

class PatrolBehavior : public AIBehavior {
public:
    void UpdateAI(AIController& component, float deltaTime) override {
        // Check state of NPC
        if (component.GetState() != AIState::Patrol) return;

        auto& route = component.GetPatrolRoute();
        if (route.empty()) return;  // No route defined

        // Get current position
        TransformComponent* t = component.GetTransformComponent();
        VelocityComponent* v = component.GetVelocityComponent();
        if (!t || !v) return;

        float posX = static_cast<float>(t->x);
        float posY = static_cast<float>(t->y);

        VectorFloat dest = route[component.GetPatrolIndex()];
        VectorFloat dir = (dest - VectorFloat{posX, posY}).Normalized();

        // Move str8 to the point
        v->dx = dir.x * component.GetSpeed();
        v->dy = dir.y * component.GetSpeed();

        // If NPC reached destination
        if ((dest - VectorFloat{posX, posY}).Length() < 0.5f) {
            // Set new index (new destination)
            int id = (component.GetPatrolIndex() + 1) % route.size();
            component.SetPatrolIndex(id);
        }
    }
};
