//
// Created by braynstorm on 2/10/18.
//

#include "Window.hpp"

namespace glee {
    void Window::callRenderCallbacks(Uint32 delta) {
        for (auto &&function : _renderCallbacks) { function(delta); }
    }

    Window::Window(const std::string &title, int x, int y, int width, int height) {
        SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO);

        _sdlWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL);
    }

    void Window::loop() {
        _running = true;

        Uint32 lastTickCount = SDL_GetTicks();

        while (_running) {
            Uint32 currentTickCount = SDL_GetTicks();
            Uint32 delta = currentTickCount - lastTickCount;

            // TODO: Comprimise for multiple missed frames
            if (delta > getFrameLength()) {
                // TODO: Check for double-buffering and clearing options
                callRenderCallbacks(delta);
            }
        }
    }

    void Window::getSize(int &width, int &height) { SDL_GetWindowSize(_sdlWindow, &width, &height); }
    void Window::setSize(int width, int height) { SDL_SetWindowSize(_sdlWindow, width, height); }

    Uint32 Window::getFrameLength() { return _frameLength; }
    void Window::setFrameLength(Uint32 newLength) { _frameLength = newLength; }

    std::string Window::getTitle() { return SDL_GetWindowTitle(_sdlWindow); }
    void Window::setTitle(const std::string &title) { SDL_SetWindowTitle(_sdlWindow, title.c_str()); }

    void Window::addRenderCallback(Window::RenderCallback callback) { _renderCallbacks.insert(callback); }

    void Window::stop() {
        // TODO: Do render-sepcific cleanup if necessery
        _running = false;
    }

    void Window::close() {
        SDL_DestroyWindow(_sdlWindow);
        SDL_Quit();
    }
} // namespace glee
