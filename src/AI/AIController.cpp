#include "AI/AIController.h"
#include <algorithm>

AIController::AIController(int maxHealth, float visionRange, bool isFriendly)
    : m_health{maxHealth}, m_maxHealth{maxHealth},
      m_visionRange{visionRange}, m_isFriendly{isFriendly},
      m_currentState{AIState::Idle} {}

void AIController::SetBehavior(std::unique_ptr<AIBehavior> newBehavior) {
    behavior = std::move(newBehavior);
}

void AIController::TakeDamage(int amount) {
    m_health -= amount;
    if (m_health <= 0) {
        m_health = 0;
        m_currentState = AIState::Dead;
    }
}

void AIController::Heal(int amount) {
    m_health = std::min(m_health + amount, m_maxHealth);
}

bool AIController::HealthLow() const {
    return m_health < (m_maxHealth * 0.3f);
}

bool AIController::CanSeeEnemy() const {
    // LATER
    return false;
}

void AIController::ToggleFriendliness() {
    m_isFriendly = !m_isFriendly;
}

bool AIController::IsFriendly() const {
    return m_isFriendly;
}

AIState AIController::GetState() const {
    return m_currentState;
}

void AIController::ChangeState(AIState newState) {
    m_currentState = newState;
}

int AIController::GetHealth() const {
    return m_health;
}

float AIController::GetVisionRange() const {
    return m_visionRange;
}

AIBehavior* AIController::GetCurrentBehavior() const {
    return behavior.get();
}

void AIController::SetTarget(EntityID entityID) {
    targetID = entityID;
}

std::optional<EntityID> AIController::GetTarget() const {
    return targetID;
}

void AIController::ClearTarget() {
    targetID.reset();
}
