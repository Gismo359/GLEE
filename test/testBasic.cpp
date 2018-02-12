#include <GL/glew.h>
#include "../src/GLEE.hpp"
#include "../src/Log.hpp"

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
    batch.addLine(Line{
        vec2{ 0.0f, 0.0f },
        vec2{ -.5f, .5f }
    }.setEndColor(0xFF00FF00u));

    batch.addLine(Line{
        vec2{ 0.5f, 0.0f },
        vec2{ -.1f, .5f }
    });
    glEnableClientState(GL_VERTEX_ARRAY);

    window.addEventCallback([](Window& window, Uint32 delta, SDL_Event event) {
        if (event.type == SDL_QUIT) window.stop();
        return false;
    });

    glClearColor(0.0, 0.2, 0.0, 1.0);

    window.addRenderCallback([&batch](Window& window, float delta) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        batch.draw(false);
    });

    window.loop();
};
