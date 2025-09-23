#include "core/Entity.h"

Entity::Entity(EntityID id) : m_id(id) {}

EntityID Entity::GetID() const {
    return m_id;
}
