#pragma once

#include <memory>
#include <optional>

#include "AIBehavior.h"
#include "utils/EntityTypes.h"
#include "utils/Vector.h"
#include "utils/AnimationUtils.h"

#include "core/ComponentStorage.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"
#include "components/HealthComponent.h"
#include "event/custom_events/DamageEvent.h"

enum class AIState {
    Idle,
    Patrol,
    Attack,
    Flee,
    Chase,
    Follow,
    Dead
};

enum class MovementMode { Walk, Run };

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
    bool CanHearEnemy() const;

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
    float GetAttackRange() const;
    int GetDamage() const;
    const std::string& GetAttackType() const;
    const std::string& GetAttackEffect() const;
    float GetAttackEffectDuration() const;
    bool GetEnabledCritical() const;
    float GetCriticalChance() const;
    float GetCriticalBonus() const;
    float GetAttackCooldown() const;

    // Basic data (setters)
    void SetVisionRange(float range);
    void SetHearingRange(float range);
    void SetFieldOfView(float fov);
    void SetSpeed(float speed);
    void SetArmor(int armor);
    void SetStamina(int stamina);
    void SetMorale(int morale);
    void SetAttackRange(float range);
    void SetDamage(int damage);
    void SetAttackType(const std::string& type);
    void SetAttackEffect(const std::string& effect);
    void SetAttackEffectDuration(float duration);
    void SetEnabledCritical(bool isCritical);
    void SetCriticalChance(float chance);
    void SetCriticalBonus(float bonus);
    void SetAttackCooldown(float cd);

    // Position & movement
    void SetPosition(const VectorFloat& pos);
    VectorFloat GetPosition() const;
    void SetDestination(const VectorFloat& dest);
    VectorFloat GetDestination() const;
    void SetVelocity(const VectorFloat& vel);
    VectorFloat GetVelocity() const;
    void SetPatrolRoute(std::vector<VectorFloat> route);
    const std::vector<VectorFloat>& GetPatrolRoute() const;
    void SetPatrolIndex(int newIndex);
    int GetPatrolIndex() const;
    void AttachComponents(TransformComponent* t, VelocityComponent* v);
    VelocityComponent* GetVelocityComponent();
    TransformComponent* GetTransformComponent();
    void SetTargetTransform(TransformComponent* t);
    TransformComponent* GetTargetTransform();
    void SetTargetHealth(HealthComponent* t);
    HealthComponent* GetTargetHealth();
    void SetDesiredDistance(float distance);
    float GetDesiredDistance();
    void SetMovementMode(MovementMode mode);
    MovementMode GetMovementMode() const;
    bool IsRunning() const;
    void SetWalkSpeed(float speed);
    void SetRunSpeed(float speed);

    // Target handling
    void SetTarget(EntityID entityID);
    std::optional<EntityID> GetTarget() const;
    void ClearTarget();

    // Faction
    void SetFaction(int factionID);
    int GetFaction() const;

    // Animations
    void SetAnimationComponent(AnimationComponent* anim);
    AnimationComponent* GetAnimationComponent();

private:
    // Stats and attack
    int m_health;
    int m_maxHealth;
    int m_armor;
    int m_stamina;
    int m_morale;
    int m_damage;
    float m_speed;
    float m_attackRange;
    float m_attackCooldown;

    std::string m_attackType;
    std::string m_attackEffect;
    float m_attackEffectDuration;
    bool m_isEnabledCritical;
    float m_criticalChance;
    float m_criticalBonus;

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
    std::vector<VectorFloat> m_patrolRoute;
    int m_currentPatrolIndex = 0;
    TransformComponent* m_transform = nullptr;
    VelocityComponent* m_velocityComp = nullptr;
    TransformComponent* m_targetTransform = nullptr;
    HealthComponent* m_targetHealth = nullptr;
    float m_desiredDistance;
    MovementMode m_movementMode = MovementMode::Walk;
    float m_walkSpeed = 1.0f;
    float m_runSpeed  = 3.0f;

    // Behavior
    std::unique_ptr<AIBehavior> behavior;

    // Target
    std::optional<EntityID> targetID;

    // Faction
    int m_factionID;

    // Animations
    AnimationComponent* m_animation;
};
