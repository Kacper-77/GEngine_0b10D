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

    // Getters
    int GetWidth() const;
    int GetHeight() const;
    bool GetIsFullscreen() const;
    bool IsRunning() const;

    // Resolution and fullscreen
    void SetResolution(int width, int height);
    void ToggleFullscreen();

    // Window properties
    void SetTitle(const std::string& title);
    void SetIcon(SDL_Surface* icon);
    void SetPosition(int x, int y);
    void CenterOnDisplay(int displayIndex);
    void SetBorderless(bool borderless);
    void SetResizable(bool resizable);

    // Cursor
    void ShowCursor(bool show);
    void SetRelativeMouseMode(bool enabled);

    // DPI / info
    float GetDPI(int displayIndex = 0) const;
    void LogWindowInfo() const;

    // Events
    void PollEvents();

private:
    SDL_Window* m_window;
    int m_width;
    int m_height;
    bool m_isRunning;
    bool m_isFullscreen;
};
