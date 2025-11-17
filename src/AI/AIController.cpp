#include "AI/AIController.h"
#include <algorithm>
#include <cmath>

// Constructor
AIController::AIController(int maxHealth, int health)
    : m_maxHealth{maxHealth},
      m_health{std::min(health, maxHealth)},
      m_isAlive{true},
      m_currentState{AIState::Idle},
      m_armor{0},
      m_stamina{100},
      m_morale{100},
      m_speed{1.0f},
      m_visionRange{10.0f},
      m_hearingRange{5.0f},
      m_fieldOfView{120.0f},
      m_isFriendly{true},
      m_isAggressive{false},
      m_factionID{0},
      m_position{0,0},
      m_velocity{0,0},
      m_destination{0,0} {}


// Behavior
void AIController::SetBehavior(std::unique_ptr<AIBehavior> newBehavior) {
    behavior = std::move(newBehavior);
}

AIBehavior* AIController::GetCurrentBehavior() const {
    return behavior.get();
}


// Commands
void AIController::TakeDamage(int amount) {
    m_health -= amount;
    if (m_health <= 0) {
        m_health = 0;
        m_isAlive = false;
        m_currentState = AIState::Dead;
    }
}

void AIController::Heal(int amount) {
    m_health = std::min(m_health + amount, m_maxHealth);
    if (m_health > 0 && !m_isAlive) {
        m_isAlive = true;
        m_currentState = AIState::Idle;
    }
}

bool AIController::HealthLow() const {
    return m_health < (m_maxHealth * 0.3f);
}


// Perception
bool AIController::CanHearEnemy() const {
    // Check condition
    if (!targetID.has_value() || !m_isAlive) return false;

    // Get TransformComponent of current target
    auto* target = m_transform;
    if (!target) return false;
    
    // Calculate distance
    VectorFloat targetPos = {static_cast<float>(target->x), static_cast<float>(target->y)};
    float distance = (targetPos - m_position).Length();
    
    return distance <= m_hearingRange;
}

bool AIController::CanSeeEnemy() const {
    // Check condition
    if (!targetID.has_value() || !m_isAlive) return false;

    // Get TransformComponent of current target
    auto* target = m_transform;
    if (!target) return false;

    // Get distance to target
    VectorFloat targetPos = {static_cast<float>(target->x), static_cast<float>(target->y)};
    VectorFloat toTarget = targetPos - m_position;

    // Check condition v2
    if (toTarget.Length() > m_visionRange) return false;

    // Get facing direction
    VectorFloat facing = m_velocity.Length() > 0 ? m_velocity.Normalized() : VectorFloat{1,0};

    // Calculate angle
    float dot = facing.Dot(toTarget.Normalized());
    dot = std::clamp(dot, -1.0f, 1.0f);
    float angle = std::acos(dot) * 180.0f / M_PI;

    return angle <= (m_fieldOfView * 0.5f);
}

// Friendliness
void AIController::ToggleFriendliness() {
    m_isFriendly = !m_isFriendly;
}

bool AIController::IsFriendly() const {
    return m_isFriendly;
}


// State management
AIState AIController::GetState() const {
    return m_currentState;
}

void AIController::ChangeState(AIState newState) {
    m_currentState = newState;
}


// Basic data (getters)
int AIController::GetHealth() const { return m_health; }
int AIController::GetMaxHealth() const { return m_maxHealth; }
float AIController::GetVisionRange() const { return m_visionRange; }
float AIController::GetHearingRange() const { return m_hearingRange; }
float AIController::GetFieldOfView() const { return m_fieldOfView; }
float AIController::GetSpeed() const { return m_speed; }
int AIController::GetArmor() const { return m_armor; }
int AIController::GetStamina() const { return m_stamina; }
int AIController::GetMorale() const { return m_morale; }


// Basic data (setters)
void AIController::SetVisionRange(float range) { m_visionRange = range; }
void AIController::SetHearingRange(float range) { m_hearingRange = range; }
void AIController::SetFieldOfView(float fov) { m_fieldOfView = fov; }
void AIController::SetSpeed(float speed) { m_speed = speed; }
void AIController::SetArmor(int armor) { m_armor = armor; }
void AIController::SetStamina(int stamina) { m_stamina = stamina; }
void AIController::SetMorale(int morale) { m_morale = morale; }


// Position & movement
void AIController::SetPosition(const VectorFloat& pos) { m_position = pos; }
VectorFloat AIController::GetPosition() const { return m_position; }

void AIController::SetDestination(const VectorFloat& dest) { m_destination = dest; }
VectorFloat AIController::GetDestination() const { return m_destination; }

void AIController::SetVelocity(const VectorFloat& vel) { m_velocity = vel; }
VectorFloat AIController::GetVelocity() const { return m_velocity; }

void AIController::SetPatrolRoute(std::vector<VectorFloat> route) {
    m_patrolRoute = route;
    m_currentPatrolIndex = 0;
}
const std::vector<VectorFloat>& AIController::GetPatrolRoute() const { return m_patrolRoute; }
void AIController::SetPatrolIndex(int newIndex) { m_currentPatrolIndex = newIndex; }
int AIController::GetPatrolIndex() const { return m_currentPatrolIndex; }

void AIController::AttachComponents(TransformComponent* t, VelocityComponent* v) {
    m_transform = t;
    m_velocityComp = v;
}
TransformComponent* AIController::GetTransformComponent() { return m_transform; }
VelocityComponent* AIController::GetVelocityComponent() { return m_velocityComp; }


// Target handling
void AIController::SetTarget(EntityID entityID) { targetID = entityID; }
std::optional<EntityID> AIController::GetTarget() const { return targetID; }
void AIController::ClearTarget() { targetID.reset(); }


// Faction
void AIController::SetFaction(int factionID) { m_factionID = factionID; }
int AIController::GetFaction() const { return m_factionID; }
