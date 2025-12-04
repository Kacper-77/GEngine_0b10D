#pragma once

#include <cstdint> 
#include <unordered_map>

#include "AIController.h"
#include "core/ISystem.h"

using ControllerID = uint64_t;

class AISystem : public ISystem {
public:
    void AddController(AIController* controller);
    void RemoveController(ControllerID id);
    AIController* GetController(ControllerID id) const;
 
    // ISystem method
    void Update(float deltaTime) override;

private:
    size_t NextControllerID_ = 1;
    std::unordered_map<ControllerID, AIController*> m_controllers;
};
