//
// Created by braynstorm on 2/10/18.
//

#include "Window.hpp"
#include "Log.hpp"

namespace glee {
    void Window::callRenderCallbacks(Uint32 delta) {
        for (auto &&function : _renderCallbacks) { function(*this, delta); }
    }

    void Window::callEventCallbacks(Uint32 delta, SDL_Event event) {
        for (auto &&function : _eventCallbacks) {
            // TODO: Possibly add a handled flag via a wrapper and make EventCallback return void?
            if (function(*this, delta, event))
                // TODO: Do something else once handled?
                break;
        }
    }

    Window::Window(const std::string &title, int x, int y, int width, int height) {
        if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
            LOG(ERROR, "SDL Failed to initialize!");
            abort();
        }

        _sdlWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL);
    }

    void Window::loop() {
        _running = true;

        Uint32 lastTick = SDL_GetTicks();

        while (_running) {
            Uint32 currentTick = SDL_GetTicks();
            Uint32 delta = currentTick - lastTick;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                // TODO: Filter or preprocess some events?
                callEventCallbacks(delta, event);
            }

            // TODO: Comprimise for multiple missed frames
            if (delta > getFrameLength()) {
                // TODO: Check for double-buffering and clearing options
                callRenderCallbacks(delta);

                SDL_GL_SwapWindow(_sdlWindow);
            }
        }
    }

    // TODO: Maybe add separate wrappers for width/height
    void Window::getSize(int &width, int &height) const { SDL_GetWindowSize(_sdlWindow, &width, &height); }

    void Window::setSize(int width, int height) { SDL_SetWindowSize(_sdlWindow, width, height); }

    Uint32 Window::getFrameLength() const { return _frameLength; }

    void Window::setFrameLength(Uint32 newLength) { _frameLength = newLength; }

    std::string Window::getTitle() const { return SDL_GetWindowTitle(_sdlWindow); }

    void Window::setTitle(const std::string &title) { SDL_SetWindowTitle(_sdlWindow, title.c_str()); }

    void Window::addRenderCallback(Window::RenderCallback callback) { _renderCallbacks.push_back(callback); }

    void Window::addEventCallback(Window::EventCallback callback) { _eventCallbacks.push_back(callback); }

    void Window::stop() {
        // TODO: Do render-specific cleanup if necessary
        _running = false;
    }

    void Window::close() {
        stop();
        SDL_DestroyWindow(_sdlWindow);
        SDL_Quit();
    }
} // namespace glee
