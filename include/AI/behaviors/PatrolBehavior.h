#pragma once

#include "AI/AIBehavior.h"
#include "AI/AIController.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

class PatrolBehavior : public AIBehavior {
public:
    void UpdateAI(AIController& component, float deltaTime) override {
        std::cout << "Patrol update\n";

        // Check state of NPC
        if (component.GetState() != AIState::Patrol) return;

        auto& route = component.GetPatrolRoute();
        if (route.empty()) return;  // No route defined

        // Get current position
        TransformComponent* t = component.GetTransformComponent();
        VelocityComponent* v = component.GetVelocityComponent();
        if (!t || !v) return;

        float posX = t->position.x;
        float posY = t->position.y;

        VectorFloat dest = route[component.GetPatrolIndex()];
        VectorFloat dir = (dest - VectorFloat{posX, posY}).Normalized();

        // Determine speed based on movement mode (walk/run)
        float speed = component.GetSpeed();
        v->dx = dir.x * speed;
        v->dy = dir.y * speed;

        // Trigger animation if available
        if (auto* anim = component.GetAnimationComponent()) {
            if (anim->stateMachine) {
                anim->stateMachine->currentState = component.IsRunning() ? "Run" : "Walk";
            }
        }

        // If NPC reached destination
        if ((dest - VectorFloat{posX, posY}).Length() < 0.5f) {
            // Set new index (new destination)
            int id = (component.GetPatrolIndex() + 1) % route.size();
            component.SetPatrolIndex(id);
        }
    }
};
