//
// Created by braynstorm on 2/10/18.
//

#ifndef GLEE_WINDOW_HPP
#define GLEE_WINDOW_HPP

#include <SDL2/SDL.h>
#include <functional>
#include <set>
#include <string>

namespace glee {
    class Window {
      private:
        using RenderCallback = std::function<void(Window& window, Uint32 delta)>;
        using EventCallback = std::function<bool(Window& window, Uint32 delta, SDL_Event event)>;
        // TODO reorder the members here to account for cache-missing.
        SDL_Window* _sdlWindow;
        std::vector<RenderCallback> _renderCallbacks;
        std::vector<EventCallback> _eventCallbacks;

        bool _running;

        Uint32 _frameLength = 1000 / 60;

        GLuint _shaderProgram = 0;
        GLint _uniformProjectionMatrix = 0;
        GLint _uniformModelViewMatrix = 0;

        void callRenderCallbacks(Uint32 delta);
        void callEventCallbacks(Uint32 delta, SDL_Event event);

      public:
        Window(const std::string& title, int x, int y, int width, int height);

        void loop();

        std::string getTitle() const;
        void setTitle(const std::string& title);

        void getSize(int& width, int& height) const;
        void setSize(int width, int height);

        Uint32 getFrameLength() const;
        void setFrameLength(Uint32 newLength);

        void addRenderCallback(RenderCallback callback);
        void addEventCallback(EventCallback callback);

        void stop();
        void close();
        void compileShaderProgram();
    };
} // namespace glee
#endif // GLEE_WINDOW_HPP
