#pragma once

#include "AIController.h"

class AIBehavior {
public:
    virtual ~AIBehavior() = default;
    virtual void UpdateAI(AIController& component, float deltaTime) = 0;
};
