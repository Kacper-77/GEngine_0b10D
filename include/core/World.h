#pragma once

// ComponentStorage
#include "IComponentStorage.h"
#include "ComponentStorage.h"

// Systems
#include "ISystem.h"
#include "systems/AudioSystem.h"
#include "systems/BoundrySystem.h"
#include "systems/CameraSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/EntityCreationSystem.h"
#include "systems/MovementSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/RenderSystem.h"
#include "systems/SurfaceBehaviorSystem.h"

// Forward declaration — Input
class InputManager;

// Forward declarations — Core
class EntityManager;
class SystemManager;
class EventBus;
class LevelManager;

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <stdexcept>

class World {
public:
    /*
        COMPONENTS
        Add, Get, Has, Remove 
    */
    template<typename T>
    void AddComponentStorage(std::unique_ptr<ComponentStorage<T>> storage) {
        m_components[typeid(T)] = std::move(storage);
    }

    template<typename T>
    ComponentStorage<T>& GetComponentStorage() {
        auto it = m_components.find(typeid(T));
        if (it == m_components.end()) {
            throw std::runtime_error("ComponentStorage for this type not found.");
        }
        return *static_cast<ComponentStorage<T>*>(it->second.get());
    }

    template<typename T>
    bool HasComponentStorage() const {
        return m_components.find(typeid(T)) != m_components.end();
    }

    template<typename T>
    void RemoveComponentStorage() {
        m_components.erase(typeid(T));
    }

    /*
        INPUT
        Add, Get, Remove
    */
    void SetInputManager(InputManager& inputManager) {
        m_inputManager = &inputManager;
    }

    InputManager& GetInputManager() {
        if (!m_inputManager) {
            throw std::runtime_error("InputManager not set.");
        }
        return *m_inputManager;
    }

    void RemoveInputManager() {
        m_inputManager = nullptr;
    }

    /*
        CORE
        1. EntityManager
        2. SystemManager
        3. EventBus
        4. LevelManager...
    */
   // EntityManager
    void SetEntityManager(EntityManager& entityManager) {
        m_entityManager = &entityManager;
    }

    EntityManager& GetEntityManager() {
        if (!m_entityManager) {
            throw std::runtime_error("EntityManager not set.");
        }
        return *m_entityManager;
    }

    void RemoveEntityManager() {
        m_entityManager = nullptr;
    }

    // SystemManager
    void SetSystemManager(SystemManager& systemManager) {
        m_systemManager = &systemManager;
    }

    SystemManager& GetSystemManager() {
        if (!m_systemManager) {
            throw std::runtime_error("SystemManager not set.");
        }
        return *m_systemManager;
    }

    void RemoveSystemManager() {
        m_systemManager = nullptr;
    }

    // EventBus
    void SetEventBus(EventBus& eventBus) {
        m_eventBus = &eventBus;
    }

    EventBus& GetEventBus() {
        if (!m_eventBus) {
            throw std::runtime_error("EventBus no set.");
        }
        return *m_eventBus;
    }

    void RemoveEventBus() {
        m_eventBus = nullptr;
    }

    // LevelManager
    void SetLevelManager(LevelManager& levelManger) {
        m_levelManager = &levelManger;
    }

    LevelManager& GetLevelManager() {
        if (!m_levelManager) {
            throw std::runtime_error("LevelManager not set.");
        }
        return *m_levelManager;
    }

    void RemoveLevelManager() {
        m_levelManager = nullptr;
    }

    /*
        SYSTEMS
        Add, Get, Remove
    */
    template<typename T>
    void AddSystem(std::unique_ptr<T> system) {
        m_systems[typeid(T)] = std::move(system);
    }

    template<typename T>
    T& GetSystem() {
        auto it = m_systems.find(typeid(T));
        if (it != m_systems.end()) {
            return *static_cast<T*>(it->second.get());
        } else {
            throw std::runtime_error("System not set.");
        }
    }

    template<typename T>
    void RemoveSystem() {
        m_systems.erase(typeid(T));
    }

private:
    // COMPONENTS
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> m_components;

    // INPUT
    InputManager* m_inputManager = nullptr;

    // CORE
    EntityManager* m_entityManager = nullptr;
    SystemManager* m_systemManager = nullptr;
    EventBus* m_eventBus = nullptr;
    LevelManager* m_levelManager = nullptr;

    // SYSTEMS
    std::unordered_map<std::type_index, std::unique_ptr<ISystem>> m_systems;
};
