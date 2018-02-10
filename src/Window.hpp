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
        using RenderCallback = std::function<void(Uint32 delta)>;
        using EventCallback = std::function<bool(Uint32 delta, SDL_Event event)>;

        SDL_Window *_sdlWindow;
        std::vector<RenderCallback> _renderCallbacks;
        std::vector<EventCallback> _eventCallbacks;

        bool _running;

        Uint32 _frameLength;

        void callRenderCallbacks(Uint32 delta);
        void callEventCallbacks(Uint32 delta, SDL_Event event);

      public:
        Window(const std::string &title, int x, int y, int width, int height);

        void loop();

        std::string getTitle();
        void setTitle(const std::string &title);

        void getSize(int &width, int &height);
        void setSize(int width, int height);

        Uint32 getFrameLength();
        void setFrameLength(Uint32 newLength);

        void addRenderCallback(RenderCallback callback);
        void addEventCallback(EventCallback callback);

        void stop();
        void close();
    };
} // namespace glee
#endif // GLEE_WINDOW_HPP
