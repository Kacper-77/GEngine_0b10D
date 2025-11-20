#pragma once

#include "core/ISystem.h"
#include "utils/EntityTypes.h"
#include "event/core/EventBus.h"
#include "event/custom_events/DamageEvent.h"
#include "core/ComponentStorage.h"
#include "components/HealthComponent.h"

class CombatSystem : public ISystem {
public:
    CombatSystem(ComponentStorage<HealthComponent>& health, EventBus& eventBus);

    // ISystem method
    void Update(float deltaTime) override;

private:
    // Required fields
    ComponentStorage<HealthComponent>& m_health;
    EventBus& m_eventBus;

    // Attack/Damage event handling
    void OnDamageEvent(const DamageEvent& e);
};
