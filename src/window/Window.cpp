#include "../include/window/Window.h"
#include <iostream>

Window::Window()
    : m_window(nullptr), m_width(0), m_height(0), m_isRunning(false) {}

Window::~Window() {
    Shutdown();
}

bool Window::Init(const std::string& title, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

SDL_Window* Window::GetSDLWindow() const {
    return m_window;
}

int Window::GetWidth() const {
    return m_width;
}

int Window::GetHeight() const {
    return m_height;
}

bool Window::GetIsFullscreen() const {
    return m_isFullscreen;
}

bool Window::IsRunning() const {
    return m_isRunning;
}

void Window::SetResolution(int width, int height) {
    m_width = width;
    m_height = height;
    SDL_SetWindowSize(m_window, width, height);
}

void Window::ToggleFullscreen() {
    Uint32 flags = SDL_GetWindowFlags(m_window);
    bool isFullscreen = flags & SDL_WINDOW_FULLSCREEN;

    if (isFullscreen) {
        SDL_SetWindowFullscreen(m_window, 0); // windowed
        m_isFullscreen = false;
    } else {
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN); // fullscreen
        m_isFullscreen = true;
    }
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
    }
}
