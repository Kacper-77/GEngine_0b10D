#pragma once

#include <memory>
#include <optional>
#include "AIBehavior.h"
#include "utils/EntityTypes.h"
#include "utils/Vector.h"
#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"

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
    AIController(int maxHealth, int health);

    // Behavior
    void SetBehavior(std::unique_ptr<AIBehavior> newBehavior);
    AIBehavior* GetCurrentBehavior() const;

    // Commands
    void TakeDamage(int amount);
    void Heal(int amount);

    bool HealthLow() const;
    bool CanSeeEnemy() const;
    bool CanHearEnemy( ComponentStorage<TransformComponent>& transforms) const;

    void ToggleFriendliness();
    bool IsFriendly() const;

    // State management
    AIState GetState() const;
    void ChangeState(AIState newState);

    // Basic data (getters)
    int GetHealth() const;
    int GetMaxHealth() const;
    float GetVisionRange() const;
    float GetHearingRange() const;
    float GetFieldOfView() const;
    float GetSpeed() const;
    int GetArmor() const;
    int GetStamina() const;
    int GetMorale() const;

    // Basic data (setters)
    void SetVisionRange(float range);
    void SetHearingRange(float range);
    void SetFieldOfView(float fov);
    void SetSpeed(float speed);
    void SetArmor(int armor);
    void SetStamina(int stamina);
    void SetMorale(int morale);

    // Position & movement
    void SetPosition(const VectorFloat& pos);
    VectorFloat GetPosition() const;
    void SetDestination(const VectorFloat& dest);
    VectorFloat GetDestination() const;
    void SetVelocity(const VectorFloat& vel);
    VectorFloat GetVelocity() const;

    // Target handling
    void SetTarget(EntityID entityID);
    std::optional<EntityID> GetTarget() const;
    void ClearTarget();

    // Faction
    void SetFaction(int factionID);
    int GetFaction() const;

private:
    // Stats
    int m_health;
    int m_maxHealth;
    int m_armor;
    int m_stamina;
    int m_morale;
    float m_speed;

    // Perception
    float m_visionRange;
    float m_hearingRange;
    float m_fieldOfView;

    // Flags
    bool m_isFriendly;
    bool m_isAggressive;
    bool m_isAlive;

    // State
    AIState m_currentState;

    // Movement
    VectorFloat m_position;
    VectorFloat m_velocity;
    VectorFloat m_destination;

    // Behavior
    std::unique_ptr<AIBehavior> behavior;

    // Target
    std::optional<EntityID> targetID;

    // Faction
    int m_factionID;
};
