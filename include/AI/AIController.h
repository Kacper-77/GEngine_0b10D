#pragma once

#include <memory>
#include <optional>
#include "AIBehavior.h"
#include "utils/EntityTypes.h"

enum class AIState {
    Idle,
    Patrol,
    Attack,
    Flee,
    Dead
};

class AIController {
public:
    // Constructor
    AIController(int maxHealth, float visionRange, bool isFriendly);

    // Setting new behavior
    void SetBehavior(std::unique_ptr<AIBehavior> newBehavior);

    // Basic commands
    void TakeDamage(int amount);
    void Heal(int amount);

    bool HealthLow() const;
    bool CanSeeEnemy() const;

    void ToggleFriendliness();
    bool IsFriendly() const;

    // State management
    AIState GetState() const;
    void ChangeState(AIState newState);

    // Basic data
    int GetHealth() const;
    float GetVisionRange() const;
    AIBehavior* GetCurrentBehavior() const;

    // Target handling
    void SetTarget(EntityID entityID);
    std::optional<EntityID> GetTarget() const;
    void ClearTarget();

private:
    int m_health;
    int m_maxHealth;
    float m_visionRange;
    bool m_isFriendly;
    AIState m_currentState;

    std::unique_ptr<AIBehavior> behavior;
    std::optional<EntityID> targetID; // target ID
};
