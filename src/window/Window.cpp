#include "window/Window.h"
#include <iostream>

Window::Window()
    : m_window(nullptr), m_width(0), m_height(0),
      m_isRunning(false), m_isFullscreen(false) {}

Window::~Window() {
    Shutdown();
}

bool Window::Init(const std::string& title, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN_DESKTOP;

    m_window = SDL_CreateWindow(title.c_str(),
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, flags);

    if (!m_window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_width = width;
    m_height = height;
    m_isRunning = true;
    m_isFullscreen = fullscreen;

    return true;
}

void Window::Shutdown() {
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
    m_isRunning = false;
}

SDL_Window* Window::GetSDLWindow() const { return m_window; }
int Window::GetWidth() const { return m_width; }
int Window::GetHeight() const { return m_height; }
bool Window::GetIsFullscreen() const { return m_isFullscreen; }
bool Window::IsRunning() const { return m_isRunning; }

void Window::SetResolution(int width, int height) {
    m_width = width;
    m_height = height;
    SDL_SetWindowSize(m_window, width, height);
}

void Window::ToggleFullscreen() {
    Uint32 flags = SDL_GetWindowFlags(m_window);
    bool isFullscreen = flags & SDL_WINDOW_FULLSCREEN;
    if (isFullscreen) {
        SDL_SetWindowFullscreen(m_window, 0);
        m_isFullscreen = false;
    } else {
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
        m_isFullscreen = true;
    }
}

void Window::SetTitle(const std::string& title) {
    SDL_SetWindowTitle(m_window, title.c_str());
}

void Window::SetIcon(SDL_Surface* icon) {
    SDL_SetWindowIcon(m_window, icon);
}

void Window::SetPosition(int x, int y) {
    SDL_SetWindowPosition(m_window, x, y);
}

void Window::CenterOnDisplay(int displayIndex) {
    SDL_Rect bounds;
    if (SDL_GetDisplayBounds(displayIndex, &bounds) == 0) {
        int x = bounds.x + (bounds.w - m_width) / 2;
        int y = bounds.y + (bounds.h - m_height) / 2;
        SDL_SetWindowPosition(m_window, x, y);
    }
}

void Window::SetBorderless(bool borderless) {
    SDL_SetWindowBordered(m_window, borderless ? SDL_FALSE : SDL_TRUE);
}

void Window::SetResizable(bool resizable) {
    SDL_SetWindowResizable(m_window, resizable ? SDL_TRUE : SDL_FALSE);
}

void Window::ShowCursor(bool show) {
    SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
}

void Window::SetRelativeMouseMode(bool enabled) {
    SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
}

float Window::GetDPI(int displayIndex) const {
    float ddpi, hdpi, vdpi;
    if (SDL_GetDisplayDPI(displayIndex, &ddpi, &hdpi, &vdpi) == 0) {
        return ddpi;
    }
    return 96.0f; // default fallback
}

void Window::LogWindowInfo() const {
    int x, y;
    SDL_GetWindowPosition(m_window, &x, &y);
    std::cout << "Window Info:\n";
    std::cout << "  Size: " << m_width << "x" << m_height << "\n";
    std::cout << "  Position: " << x << "," << y << "\n";
    std::cout << "  Fullscreen: " << (m_isFullscreen ? "Yes" : "No") << "\n";
    std::cout << "  DPI: " << GetDPI() << "\n";
}

void Window::PollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_isRunning = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            m_isRunning = false;
        }
        if (event.type == SDL_WINDOWEVENT) {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    m_width = event.window.data1;
                    m_height = event.window.data2;
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    m_isRunning = false;
                    break;
            }
        }
    }
}
