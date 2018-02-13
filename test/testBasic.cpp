#include "../src/GLEE.hpp"
#include "../src/Log.hpp"

using glee::Line;
using glee::LineBatch;
using glee::Window;

using glee::Line;
using glee::LineBatch;
using glm::vec2;

using glm::vec2;

Window CreateWindow() {

    Window window{"GLEE Test Window", 10, 10, 800, 600};

    //    LineBatch batch;
    glee::compileShaderProgram();

    window.addRenderHandler(
        [](uint32_t delta, glee::CallbackData data) {
            static auto line1 = Line{vec2{1.0, 1.0}, vec2{-1.0, -1.0}};
            line1.setColors(0xFFFFFFFF, 0xFF00FF00);
            line1.draw();
        },
        nullptr);
    return window;
}

int main() {
    // TODO: Fix my clang-format so stuff arent so ugly, lol
    glee::Init();

    Window win1 = CreateWindow();
    Window win2 = CreateWindow();

    glee::AddEventCallback(
        [](uint32_t delta, SDL_Event event, glee::CallbackData data) {
            switch (event.type) {
                case SDL_QUIT: glee::StopLoop(); return true;

                case SDL_KEYDOWN:
                case SDL_KEYUP: {
                    SDL_Scancode key = event.key.keysym.scancode;
                    if (event.key.state == SDL_PRESSED) {
                        LOG(INFO, "You pressed down the %s key", SDL_GetScancodeName(key));
                    } else {
                        LOG(INFO, "You lifted up the %s key", SDL_GetScancodeName(key));
                    }
                    break;
                }
            }

            return false;
        },
        nullptr);

    glee::StartLoop();
    glee::Quit();
};
