#ifndef EVENTHANDLERS_HPP
#define EVENTHANDLERS_HPP

#include <SDL2/SDL_events.h>
#include <functional>

namespace glee {
    // TODO: Maybe use custom type for callback data in the future rather than raw pointer
    using CallbackData = void *;
    // TODO: Use proper container/class for keeping track of handler objects
    // TODO: Use custom event type rather than SDL_Event with more preprocessing, and maybe split it into per-window events and global ones
    using EventHandler = std::function<bool(uint32_t delta, SDL_Event event, CallbackData data)>;
    using RenderHandler = std::function<void(uint32_t delta, CallbackData data)>;

    using EventHandlerPair = std::pair<EventHandler, CallbackData>;
    using RenderHandlerPair = std::pair<RenderHandler, CallbackData>;

    // TODO: Implement more general callback system here
} // namespace glee

#endif // EVENTHANDLERS_HPP
