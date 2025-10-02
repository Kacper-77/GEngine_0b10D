#pragma once

struct ISystem {
    virtual ~ISystem() = default;
    virtual void Update(float deltaTime) = 0;
};
