#include "../src/GLEE.hpp"

using namespace glee;

int main() {
    Window window{ "Title", 500, 500, 500, 500 };
    window.addEventCallback([](Window& window, Uint32 delta, SDL_Event event) {
        if (event.type == SDL_QUIT) window.stop();
        return false;
    });

    glClearColor(0.0, 0.2, 0.0, 1.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    GLuint vbo = 0;

    float data[]{
        0.0, 1.0,
        1.0, 0.0,
        255.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, ((4 + 2)) * 2 * sizeof(float), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (GLvoid*) 0
    );

    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        0,
        (GLvoid*) ((2 * 2) * sizeof(float))
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    window.addRenderCallback([&vao](Window& window, float delta) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

    });

    window.loop();
}
