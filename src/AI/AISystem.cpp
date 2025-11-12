#include "AI/AISystem.h"

// Setters and getters
void AISystem::AddController(AIController* controller) {
    m_controllers[NextControllerID_++] = controller;
}

void AISystem::RemoveController(ControllerID id) {
    m_controllers.erase(id);
}

AIController* AISystem::GetController(ControllerID id) const {
    auto it = m_controllers.find(id);
    return it != m_controllers.end() ? it->second : nullptr;
}

// Update state (external not in SystemManager)
void AISystem::Update(float deltaTime) {
    for (auto& [id, controller] : m_controllers) {
        if (controller && controller->GetCurrentBehavior()) {
            controller->GetCurrentBehavior()->UpdateAI(*controller, deltaTime);
        }
    }
}