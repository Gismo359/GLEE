#include "../src/GLEE.hpp"

using glee::Window;

int main() {
    Window window{"GLEE Test Window", 10, 10, 800, 600};

    window.addEventCallback([](Window &window, Uint32 delta, SDL_Event event) {
        if (event.type == SDL_QUIT) window.stop();
        return false;
    });

    window.loop();
};
