#pragma once

#include "Entity.h"

class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;
    virtual void Remove(EntityID id) = 0;
};