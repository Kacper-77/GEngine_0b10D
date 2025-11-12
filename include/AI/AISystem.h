#pragma once

#include <cstdint> 
#include <unordered_map>
#include "AIController.h"

using ControllerID = uint64_t;

class AISystem {
public:
    void AddController(AIController* controller);
    void RemoveController(ControllerID id);
    AIController* GetController(ControllerID id) const;
    void Update(float deltaTime);

private:
    size_t NextControllerID_ = 1;
    std::unordered_map<ControllerID, AIController*> m_controllers;
};
