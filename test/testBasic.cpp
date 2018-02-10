#include "../src/GLEE.hpp"

using glee::Window;
using glee::Line;
using glee::LineBatch;

using glm::vec2;
using glee::Line;
using glee::LineBatch;

using glm::vec2;

int main() {
    Window window{ "GLEE Test Window", 10, 10, 800, 600 };

    LineBatch batch;
    auto line1 = Line{
        vec2{ 0.0, 0.0 },
        vec2{ 100.0, 100.0 }
    }.setColors(
        0xFFFFFFFF,
        0xFF00FF00
    );

    batch.addLine(line1);

    window.addEventCallback([](Window& window, Uint32 delta, SDL_Event event) {
        if (event.type == SDL_QUIT) window.stop();
        return false;
    });

    window.addRenderCallback([&batch](Window& window, float delta) {
        batch.draw();
    });

    window.loop();
};
