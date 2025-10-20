#include "input/InputManager.h"

// Bind keys
void InputManager::Bind(const std::string& action, SDL_Scancode key) {
    m_bindings[action] = key;
}

// Unbind keys
void InputManager::Unbind(const std::string& action) {
    m_bindings.erase(action);
}

// Update state one per frame
void InputManager::Update() {
    m_currentKeys = SDL_GetKeyboardState(nullptr);  // Get a snapshot of the current state of the keyboard
    std::copy(m_currentKeys, m_currentKeys + SDL_NUM_SCANCODES, m_previousKeys);  // Set that snapshot
}

// Check that action is pressed and get bool
bool InputManager::IsActionPressed(const std::string& action) const {
    if (auto key = GetKeyForAction(action)) {
        return m_currentKeys[*key] && !m_previousKeys[*key];
    }
    return false;
}

// Check that action is held and call it
bool InputManager::IsActionHeld(const std::string& action) const {
    if (auto key = GetKeyForAction(action)) {
        return m_currentKeys[*key];
    }
    return false;
}

// Check that action was released
bool InputManager::IsActionReleased(const std::string& action) const {
    if (auto key = GetKeyForAction(action)) {
        return !m_currentKeys[*key] && m_previousKeys[*key];
    }
    return false;
}

// Helper to avoid boilerplate
std::optional<SDL_Scancode> InputManager::GetKeyForAction(const std::string& action) const {
    auto it = m_bindings.find(action);
    if (it == m_bindings.end()) return std::nullopt;
    return it->second;
}