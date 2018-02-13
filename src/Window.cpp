//
// Created by braynstorm on 2/10/18.
//

#include "GLEE.hpp"

#include <GL/glew.h>
#include "Window.hpp"
#include "Log.hpp"

namespace glee {
    std::set<Window *> Window::_windowTracker;

    void Window::Render(uint32_t delta) {
        for (auto &&window : _windowTracker) { window->render(delta); }
    }

    Window::Window(const std::string &title, int x, int y, int width, int height) {

        _sdlWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL);
        if (!_sdlWindow) { LOG(ERROR, "Failed to initialize window: %s", SDL_GetError()); }

        _sdlGlContext = SDL_GL_CreateContext(_sdlWindow);
        if (!_sdlGlContext) { LOG(ERROR, "Failed to initialize OpenGL context: %s", SDL_GetError()); }
        glewExperimental = true;
        auto error = glewInit();

        if (error) { LOG(INFO, "%s", glewGetErrorString(error)); }

        // Swap buffers at creation
        swapBuffers();
        makeCurrentContext();

        _windowTracker.insert(this);
    }

    // TODO: Maybe add separate wrappers for width/height
    void Window::getSize(int &width, int &height) const { SDL_GetWindowSize(_sdlWindow, &width, &height); }

    void Window::setSize(int width, int height) { SDL_SetWindowSize(_sdlWindow, width, height); }

    void Window::swapBuffer() { SDL_GL_SwapWindow(this->_sdlWindow); }

    void Window::addRenderHandler(RenderHandler callback, CallbackData data) { this->_renderCallbacks.push_back(std::make_pair(callback, data)); }

    void Window::render(uint32_t delta) const {
        makeCurrentContext();
        for (auto &&pair : this->_renderCallbacks) { pair.first(delta, pair.second); }
        swapBuffers();
    }

    void Window::swapBuffers() const { SDL_GL_SwapWindow(this->_sdlWindow); }

    int Window::makeCurrentContext() const {
        auto error = SDL_GL_MakeCurrent(this->_sdlWindow, this->_sdlGlContext);
        if (error) { LOG(ERROR, "Could not set OpenGL context to window: %s", SDL_GetError()); }
        return error;
    }

    std::string Window::getTitle() const { return SDL_GetWindowTitle(_sdlWindow); }

    void Window::setTitle(const std::string &title) { SDL_SetWindowTitle(_sdlWindow, title.c_str()); }

    uint32_t Window::getWindowId() const {
        uint32_t windowId = SDL_GetWindowID(this->_sdlWindow);
        if (!windowId) { LOG(ERROR, "Enable to retrieve window ID: %s", SDL_GetError()); }
        return windowId;
    }

    bool Window::operator<(Window other) const { return this->_sdlWindow < other._sdlWindow; }

    bool Window::operator>(Window other) const { return this->_sdlWindow > other._sdlWindow; }

    bool Window::operator==(Window other) const { return this->_sdlWindow == other._sdlWindow; }

    Window::~Window() {
        _windowTracker.erase(_windowTracker.find(this));

        SDL_GL_DeleteContext(this->_sdlGlContext);
        SDL_DestroyWindow(this->_sdlWindow);
    }

} // namespace glee
