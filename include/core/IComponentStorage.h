#pragma once

#include "utils/EntityTypes.h"

class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;
    virtual void Remove(EntityID id) = 0;
};