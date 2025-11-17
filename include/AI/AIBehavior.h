#pragma once

class AIController;

class AIBehavior {
public:
    virtual ~AIBehavior() = default;
    virtual void UpdateAI(AIController& component, float deltaTime) = 0;
};
