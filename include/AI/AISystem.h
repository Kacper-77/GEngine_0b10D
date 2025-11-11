#pragma once

#include <unordered_map>
#include "AIController.h"

class AISystem {
public:
    void AddController(AIController* controller);
    void RemoveController(size_t id);
    AIController* GetController(size_t id) const;
    void Update(float deltaTime);

private:
    size_t NextControllerID_ = 1;
    std::unordered_map<size_t, AIController*> m_controllers;
};
