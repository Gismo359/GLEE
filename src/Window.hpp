//
// Created by braynstorm on 2/10/18.
//

#ifndef GLEE_WINDOW_HPP
#define GLEE_WINDOW_HPP

#include "EventHandlers.hpp"

#include <SDL2/SDL.h>
#include <string>

namespace glee {
    class Window {
      private:
        static std::vector<Window *> _windowTracker;

        // TODO reorder the members here to account for cache-missing.
        SDL_Window *_sdlWindow;
        SDL_GLContext _sdlGlContext;

        std::vector<RenderHandlerPair> _renderCallbacks;

      public:
        static void Render(uint32_t delta);

        Window(const std::string &title, int x, int y, int width, int height);
        Window(const Window &other) = delete;
        Window(Window &&other) = default;

        std::string getTitle() const;
        void setTitle(const std::string &title);

        void getSize(int &width, int &height) const;
        void setSize(int width, int height);

        void swapBuffer();

        void addRenderHandler(RenderHandler callback, CallbackData data);
        void render(uint32_t delta) const;

        void makeCurrentContext() const;

        uint32_t getWindowId() const;

        // For sorting in sets and completeness
        bool operator<(Window other) const;
        bool operator>(Window other) const;
        bool operator==(Window other) const;

        ~Window();
    };
} // namespace glee
#endif // GLEE_WINDOW_HPP
