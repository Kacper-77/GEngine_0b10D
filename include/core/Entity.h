#pragma once
#include <cstdint>

using EntityID = std::uint32_t;

class Entity {
public:
    explicit Entity(EntityID id);
    EntityID GetID() const;

private:
    EntityID m_id;
};
