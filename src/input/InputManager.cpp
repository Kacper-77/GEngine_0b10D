#include "input/InputManager.h"

void InputManager::Bind(const std::string& action, SDL_Scancode key) {
    m_bindings[action] = key;
}

void InputManager::Unbind(const std::string& action) {
    m_bindings.erase(action);
}

void InputManager::Update() {
    m_currentKeys = SDL_GetKeyboardState(nullptr);
    std::copy(m_currentKeys, m_currentKeys + SDL_NUM_SCANCODES, m_previousKeys);
}

bool InputManager::IsActionPressed(const std::string& action) const {
    auto it = m_bindings.find(action);
    if (it == m_bindings.end()) return false;
    return m_currentKeys[it->second] && !m_previousKeys[it->second];
}

bool InputManager::IsActionHeld(const std::string& action) const {
    auto it = m_bindings.find(action);
    if (it == m_bindings.end()) return false;
    return m_currentKeys[it->second];
}

bool InputManager::IsActionReleased(const std::string& action) const {
    auto it = m_bindings.find(action);
    if (it == m_bindings.end()) return false;
    return !m_currentKeys[it->second] && m_previousKeys[it->second];
}