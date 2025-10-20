#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>
#include <optional>

class InputManager {
public:
    // Binding
    void Bind(const std::string& action, SDL_Scancode key);
    void Unbind(const std::string& action);

    // Update once per frame
    void Update();

    // Check key info
    bool IsActionPressed(const std::string& action) const;
    bool IsActionHeld(const std::string& action) const;
    bool IsActionReleased(const std::string& action) const;

private:
    // Helper
    std::optional<SDL_Scancode> GetKeyForAction(const std::string& action) const;

    std::unordered_map<std::string, SDL_Scancode> m_bindings;
    const Uint8* m_currentKeys = nullptr;
    Uint8 m_previousKeys[SDL_NUM_SCANCODES] = { 0 };
};
