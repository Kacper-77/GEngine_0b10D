#pragma once

#include "utils/EntityTypes.h"

// Virtual
class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;
    virtual void Remove(EntityID id) = 0;
};