#pragma once

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    Window();
    ~Window();

    bool Init(const std::string& title, int width, int height, bool fullscreen);
    void Shutdown();

    SDL_Window* GetSDLWindow() const;

    int GetWidth() const;
    int GetHeight() const;
    bool GetIsFullscreen() const;
    bool IsRunning() const;
    void SetResolution(int width, int height);
    void ToggleFullscreen();
    void PollEvents();

private:
    SDL_Window* m_window;
    int m_width;
    int m_height;
    bool m_isRunning;
    bool m_isFullscreen;
};
